/****************************************************************************
** This file is part of motorProto and has been modified from the original.
**
** Copyright (C) 2012 Denis Shienkov <denis.shienkov@gmail.com>
** Contact: https://www.qt.io/licensing/
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
****************************************************************************/

#include <QSerialPort>
#include <QTime>

#include "receiverThread.h"


ReceiverThread::ReceiverThread(QObject *parent) : QThread(parent)
{
}


ReceiverThread::~ReceiverThread()
{
    m_mutex.lock();
    m_quit = true;
    m_mutex.unlock();
    wait();
}


void ReceiverThread::startReceiver(const QString &portName, int waitTimeout, const QString &response)
{
    const QMutexLocker locker(&m_mutex);
    m_portName = portName;
    m_waitTimeout = waitTimeout;
    m_response = response;

    if (!isRunning())
        start();
}


void ReceiverThread::run()
{
    bool currentPortNameChanged = false;

    m_mutex.lock();

    QString currentPortName;
    if (currentPortName != m_portName)
    {
        currentPortName = m_portName;
        currentPortNameChanged = true;
    }

    int currentWaitTimeout = m_waitTimeout;
    QString currentResponse = m_response;
    m_mutex.unlock();

    QSerialPort serial;

    while (!m_quit)
    {
        if (currentPortNameChanged)
        {
            serial.close();
            serial.setPortName(currentPortName);

            if (!serial.open(QIODevice::ReadWrite))
            {
                emit error(tr("Can't open %1, error code %2")
                           .arg(m_portName).arg(serial.error()));
                return;
            }
        }

        if (serial.waitForReadyRead(currentWaitTimeout))
        {
            // read request
            QByteArray requestData = serial.readAll();
            /* this was 10 but was causing a defect where the entire buffer
             * could not be read reliably within the timeout */
            while (serial.waitForReadyRead(currentWaitTimeout))
                requestData += serial.readAll();

            // write response
            const QByteArray responseData = currentResponse.toUtf8();
            serial.write(responseData);
            if (serial.waitForBytesWritten(m_waitTimeout))
            {
                const QString request = QString::fromUtf8(requestData);
                emit this->request(request);
            }
            else
            {
                emit timeout(tr("Wait write response timeout %1")
                             .arg(QTime::currentTime().toString()));
            }

        }
        else
        {
            emit timeout(tr("Wait read request timeout %1")
                         .arg(QTime::currentTime().toString()));
        }

        m_mutex.lock();
        if (currentPortName != m_portName)
        {
            currentPortName = m_portName;
            currentPortNameChanged = true;
        }
        else
        {
            currentPortNameChanged = false;
        }

        currentWaitTimeout = m_waitTimeout;
        currentResponse = m_response;
        m_mutex.unlock();
    }
}

