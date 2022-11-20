/*******************************************************************************

    Copyright 2022 Tyler Niles

    This file is part of motorProto. 

    Created by referencing examples from Qt and the online Qt documentation. 

    This program is free software: you can redistribute it and/or modify it 
    under the terms of the GNU General Public License as published by the Free 
    Software Foundation, either version 3 of the License, or (at your option) 
    any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT 
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
    FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for 
    more details.

    You should have received a copy of the GNU General Public License along 
    with this program. If not, see <https://www.gnu.org/licenses/>. 

*******************************************************************************/

#include <QtWidgets>
#include <QComboBox>
#include <QDebug>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSerialPortInfo>
#include <QSpinBox>

#include "motorGui.h"


/* Main Application Window */
MotorGui::MotorGui()
{
    /* Common */
    serialPortLabel     = new QLabel(tr("Select port:"));
    serialPortComboBox  = new QComboBox;
    timeoutLabel        = new QLabel(tr("Timeout (ms):"));
    timeoutSpinBox      = new QSpinBox;

    /* For Tx */
    requestLabel        = new QLabel(tr("Transmit:"));
    requestLineEdit     = new QLineEdit(tr("Send 'RTS' to enable"));
    trafficLabel        = new QLabel(tr("No traffic."));
    statusLabel         = new QLabel(tr("Status: Not running."));
    runButton           = new QPushButton(tr("Start Tx"));

    /* For Rx */
    responseLabelRx         = new QLabel(tr("Response:"));
    responseLineEditRx      = new QLineEdit(tr("<Data Received>"));
    trafficLabelRx          = new QLabel(tr("No traffic."));
    statusLabelRx           = new QLabel(tr("Status: Not running."));
    runButtonRx             = new QPushButton(tr("Start Rx"));
 
    /* Go gather port info for currently connected devices.
     * XXX - Need to add functionality to detect dynamic changes since
     * as written this is done only once on app startup. */
    const auto sInfo = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : sInfo)
    {
        serialPortComboBox->addItem(info.portName());
    }

    createMenu();
    createHorizontalGroupBoxSerialPortSelect();
    createHorizontalGroupBoxSerialTx();
    createHorizontalGroupBoxSerialRx();
    createHorizontalGroupBoxCmds();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setMenuBar(menuBar);
    mainLayout->addWidget(horizontalGroupBoxSerialPortSelect);
    mainLayout->addWidget(horizontalGroupBoxSerialTx);
    mainLayout->addWidget(horizontalGroupBoxSerialRx);
    mainLayout->addWidget(horizontalGroupBoxCmds);
    setLayout(mainLayout);

    setWindowTitle(tr("Prototype Motor Controller"));
}


void MotorGui::createMenu()
{
    menuBar = new QMenuBar;

    fileMenu = new QMenu(tr("&File"), this);
    exitAction = fileMenu->addAction(tr("E&xit"));
    menuBar->addMenu(fileMenu);

    aboutMenu = new QMenu(tr("&About"), this);
    aboutAction = aboutMenu->addAction(tr("About Motor Proto"));
    menuBar->addMenu(aboutMenu);

    connect(exitAction, &QAction::triggered, this, &QDialog::accept);
    // About exits app. Temporary until I create an About pop up.
    connect(aboutAction, &QAction::triggered, this, &QDialog::accept);
}


void MotorGui::createHorizontalGroupBoxCmds()
{
    horizontalGroupBoxCmds = new QGroupBox(tr("Commands"));

    QHBoxLayout *layout = new QHBoxLayout;

    int ii = 0;
    buttons[ii++] = new QPushButton(tr("Reverse"));
    buttons[ii++] = new QPushButton(tr("Stop"));
    buttons[ii++] = new QPushButton(tr("Forward"));
    for (ii = 0; ii < NUM_MOTOR_BUTTONS; ii++) {
        layout->addWidget(buttons[ii]);
    }

    horizontalGroupBoxCmds->setLayout(layout);
}


void MotorGui::createHorizontalGroupBoxSerialPortSelect()
{
    horizontalGroupBoxSerialPortSelect = new QGroupBox(tr("Serial Port"));

    auto *layout = new QGridLayout;
    layout->addWidget(serialPortLabel);
    layout->addWidget(serialPortComboBox,   0, 1);
    layout->addWidget(timeoutLabel,         1, 0);
    layout->addWidget(timeoutSpinBox,       1, 1);

    /* Configure Spinbox, ms */
    timeoutSpinBox->setRange(1, 5000);
    timeoutSpinBox->setValue(2000);

    horizontalGroupBoxSerialPortSelect->setLayout(layout);

    serialPortComboBox->setFocus();
}


void MotorGui::createHorizontalGroupBoxSerialTx()
{
    horizontalGroupBoxSerialTx = new QGroupBox(tr("Serial Port Tx"));

    auto *layout = new QGridLayout;
    layout->addWidget(requestLabel,         0, 0);
    layout->addWidget(requestLineEdit,      0, 1, 1, -1);
    layout->addWidget(trafficLabel,         1, 0, 1, 4);
    layout->addWidget(statusLabel,          2, 0, 1, 5);
    layout->addWidget(runButton,            3, 0, 1, -1);

    horizontalGroupBoxSerialTx->setLayout(layout);

    connect(runButton, &QPushButton::clicked,   this, &MotorGui::transaction);
    connect(&sthread,  &SenderThread::response, this, &MotorGui::showResponse);
    connect(&sthread,  &SenderThread::error,    this, &MotorGui::processError);
    connect(&sthread,  &SenderThread::timeout,  this, &MotorGui::processTimeout);
}


void MotorGui::createHorizontalGroupBoxSerialRx()
{
    horizontalGroupBoxSerialRx = new QGroupBox(tr("Serial Port Rx"));

    auto *layout = new QGridLayout;
    layout->addWidget(responseLabelRx,      0, 0);
    layout->addWidget(responseLineEditRx,   0, 1, 1, -1);
    layout->addWidget(trafficLabelRx,       1, 0, 1, 4);
    layout->addWidget(statusLabelRx,        2, 0, 1, 5);
    layout->addWidget(runButtonRx,          3, 0, 1, -1);

    horizontalGroupBoxSerialRx->setLayout(layout);

    connect(runButtonRx, &QPushButton::clicked,     this, &MotorGui::startReceiver);
    connect(&rthread,    &ReceiverThread::request,  this, &MotorGui::showRequest);
    connect(&rthread,    &ReceiverThread::error,    this, &MotorGui::processErrorRx);
    connect(&rthread,    &ReceiverThread::timeout,  this, &MotorGui::processTimeoutRx);

    connect(timeoutSpinBox, &QSpinBox::textChanged, this, &MotorGui::setControlsEnabledRx);
    connect(responseLineEditRx, &QLineEdit::textChanged, this, &MotorGui::setControlsEnabledRx);
    connect(serialPortComboBox, &QComboBox::currentIndexChanged, \
            this, &MotorGui::setControlsEnabledRx);
}


/*******************************************************************************
 * Tx Section
 ******************************************************************************/
void MotorGui::transaction()
{
    //qDebug() << ("TX-transact!");
    setControlsEnabled(false);

    statusLabel->setText(tr("Status: Running, connected to port %1.")
                           .arg(serialPortComboBox->currentText()));

    //qDebug() << ("TX-transact: Sending - ") << (requestLineEdit->text());

    sthread.transaction(serialPortComboBox->currentText(),
                         timeoutSpinBox->value(),
                         requestLineEdit->text());
}


void MotorGui::showResponse(const QString &str)
{
    setControlsEnabled(true);

    trafficLabel->setText(tr("Traffic, transaction #%1:"
                               "\n\r-request: %2"
                               "\n\r-response: %3")
                            .arg(++transactionCount)
                            .arg(requestLineEdit->text())
                            .arg(str));
    qDebug() << ("TX-showresp: #") << transactionCount << (" Contents: ") << str;
}


void MotorGui::processError(const QString &str)
{
    //qDebug() << ("TX-error!");
    setControlsEnabled(true);
    statusLabel->setText(tr("Status: Not running, %1.").arg(str));
    trafficLabel->setText(tr("No traffic."));
}


void MotorGui::processTimeout(const QString &str)
{
    //qDebug() << ("TX-timeout!");
    setControlsEnabled(true);
    trafficLabel->setText(tr("No traffic."));
    statusLabel->setText(tr("Status: Running, %1.").arg(str));
}


void MotorGui::setControlsEnabled(bool enable)
{
    runButton->setEnabled(enable);
    serialPortComboBox->setEnabled(enable);
    timeoutSpinBox->setEnabled(enable);
    requestLineEdit->setEnabled(enable);
}

/******************************************************************************/
/* End Tx Section *************************************************************/
/******************************************************************************/


/*******************************************************************************
 * Rx Section
 ******************************************************************************/
void MotorGui::startReceiver()
{
    //qDebug() << ("Rx go!");
    runButtonRx->setEnabled(false);
    statusLabel->setText(tr("Status: Running, connected to port %1.")
                           .arg(serialPortComboBox->currentText()));
    rthread.startReceiver(serialPortComboBox->currentText(),
                        timeoutSpinBox->value(),
                        responseLineEditRx->text());
}


void MotorGui::showRequest(const QString &s)
{
    //qDebug() << ("Rx-showRequest!");
    trafficLabelRx->setText(tr("Traffic, transaction #%1:"
                               "\n\r-Received: %2")
                               /*"\n\r-response: %3")*/
                            .arg(++transactionCountRx)
                            .arg(s));
                            /*.arg(responseLineEditRx->text()));*/
}


void MotorGui::processErrorRx(const QString &str)
{
    //qDebug() << ("Rx-error!");
    setControlsEnabledRx();
    trafficLabelRx->setText(tr("No traffic."));
    statusLabelRx->setText(tr("Status: Not running, %1.").arg(str));
}


void MotorGui::processTimeoutRx(const QString &str)
{
    //qDebug() << ("Rx-timeout!");
    setControlsEnabledRx();
    trafficLabelRx->setText(tr("No traffic."));
    statusLabelRx->setText(tr("Status: Running, %1.").arg(str));
}


void MotorGui::setControlsEnabledRx()
{
    /* Arg was being passed in as a bool (like in tx func) but this caused a mismatch in the
     * signal-slot connects */
    runButtonRx->setEnabled(true);
}

/******************************************************************************/
/* End Rx Section *************************************************************/
/******************************************************************************/

