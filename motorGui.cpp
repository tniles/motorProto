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
    createHBoxSerialPortSelect();
    createHBoxSerialTx();
    createHBoxSerialRx();
    createHBoxDcMotorCmds();
    createHBoxServoMotorCmds();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setMenuBar(menuBar);
    mainLayout->addWidget(hBoxSerialPortSelect);
    mainLayout->addWidget(hBoxSerialTx);
    mainLayout->addWidget(hBoxSerialRx);
    mainLayout->addWidget(hBoxDcMotorCmds);
    mainLayout->addWidget(hBoxServoMotorCmds);
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
    aboutAction = aboutMenu->addAction(tr("About MotorProto"));
    aboutQtAction = aboutMenu->addAction(tr("About Qt"));
    menuBar->addMenu(aboutMenu);

    menuBar->addMenu(aboutMenu);

    connect(exitAction, &QAction::triggered, this, &QDialog::accept);
    connect(aboutAction, &QAction::triggered, this, &MotorGui::showAbout);
    connect(aboutQtAction, &QAction::triggered, this, &MotorGui::showAboutQt);
}


//#define USE_NATIVE_QT_ABOUT
void MotorGui::showAbout()
{
    QString myTitle = "<b>About MotorProto<b>";
    QString myText  = "<center><i>MotorProto provides an interface for delivering commands to various motor types for prototyping purposes.<i><center>";

    QMessageBox msgBox;
    msgBox.setWindowModality( Qt::NonModal );

#ifdef USE_NATIVE_QT_ABOUT
    msgBox.about( this, myTitle, myText );
#else
    msgBox.setWindowTitle( myTitle );
    msgBox.setText( myText );
    msgBox.setStandardButtons( QMessageBox::Close );
    msgBox.setDefaultButton( QMessageBox::Close );
    msgBox.exec();
#endif
}


void MotorGui::showAboutQt()
{
    QMessageBox msgBox;
    msgBox.aboutQt( this, "<b>About Qt<b>" );
}


void MotorGui::createHBoxDcMotorCmds()
{
    hBoxDcMotorCmds = new QGroupBox(tr("BLDC Commands"));

    QHBoxLayout *layout = new QHBoxLayout;

    int ii = 0;
    buttons[ii++] = new QPushButton(tr("Reverse"));
    buttons[ii++] = new QPushButton(tr("Stop"));
    buttons[ii++] = new QPushButton(tr("Forward"));
    for (ii = 0; ii < NUM_MOTOR_BUTTONS; ii++) {
        layout->addWidget(buttons[ii]);
    }

    hBoxDcMotorCmds->setLayout(layout);

    ii = 0;
    connect(buttons[ii++], &QPushButton::clicked, this, &MotorGui::motorDcSendReverse);
    connect(buttons[ii++], &QPushButton::clicked, this, &MotorGui::motorDcSendStop);
    connect(buttons[ii++], &QPushButton::clicked, this, &MotorGui::motorDcSendForward);
}


void MotorGui::createHBoxServoMotorCmds()
{
    hBoxServoMotorCmds = new QGroupBox(tr("Servo Commands"));

    QHBoxLayout *layout = new QHBoxLayout;

    int ii = 0;
    buttons[ii++] = new QPushButton(tr("CCW"));
    buttons[ii++] = new QPushButton(tr("Stop"));
    buttons[ii++] = new QPushButton(tr("CW"));
    for (ii = 0; ii < NUM_MOTOR_BUTTONS; ii++) {
        layout->addWidget(buttons[ii]);
    }

    hBoxServoMotorCmds->setLayout(layout);

    ii = 0;
    connect(buttons[ii++], &QPushButton::clicked, this, &MotorGui::motorSrvSendCcw);
    connect(buttons[ii++], &QPushButton::clicked, this, &MotorGui::motorSrvSendStop);
    connect(buttons[ii++], &QPushButton::clicked, this, &MotorGui::motorSrvSendCw);
}


void MotorGui::createHBoxSerialPortSelect()
{
    hBoxSerialPortSelect = new QGroupBox(tr("Serial Port"));

    auto *layout = new QGridLayout;
    layout->addWidget(serialPortLabel);
    layout->addWidget(serialPortComboBox,   0, 1);
    layout->addWidget(timeoutLabel,         1, 0);
    layout->addWidget(timeoutSpinBox,       1, 1);

    /* Configure Spinbox, ms */
    timeoutSpinBox->setRange(1, 5000);
    timeoutSpinBox->setValue(2000);

    hBoxSerialPortSelect->setLayout(layout);

    serialPortComboBox->setFocus();
}


void MotorGui::createHBoxSerialTx()
{
    hBoxSerialTx = new QGroupBox(tr("Serial Port Tx"));

    auto *layout = new QGridLayout;
    layout->addWidget(requestLabel,         0, 0);
    layout->addWidget(requestLineEdit,      0, 1, 1, -1);
    layout->addWidget(trafficLabel,         1, 0, 1, 4);
    layout->addWidget(statusLabel,          2, 0, 1, 5);
    layout->addWidget(runButton,            3, 0, 1, -1);

    hBoxSerialTx->setLayout(layout);

    // Credit: 
    // https://stackoverflow.com/a/22411267/1228878
    // https://techoverflow.net/2018/02/20/how-to-fix-gcc-error-the-lambda-has-no-capture-default/
    QString payload = "";
    connect(runButton, &QPushButton::clicked,   this, [&](){ transaction(payload); });
    connect(&sthread,  &SenderThread::response, this, &MotorGui::showResponse);
    connect(&sthread,  &SenderThread::error,    this, &MotorGui::processError);
    connect(&sthread,  &SenderThread::timeout,  this, &MotorGui::processTimeout);
}


void MotorGui::createHBoxSerialRx()
{
    hBoxSerialRx = new QGroupBox(tr("Serial Port Rx"));

    auto *layout = new QGridLayout;
    layout->addWidget(responseLabelRx,      0, 0);
    layout->addWidget(responseLineEditRx,   0, 1, 1, -1);
    layout->addWidget(trafficLabelRx,       1, 0, 1, 4);
    layout->addWidget(statusLabelRx,        2, 0, 1, 5);
    layout->addWidget(runButtonRx,          3, 0, 1, -1);

    hBoxSerialRx->setLayout(layout);

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
void MotorGui::transaction(QString &payload)
{
    //qDebug() << ("TX-transact!");
    setControlsEnabled(false);

    statusLabel->setText(tr("Status: Running, connected to port %1.")
                           .arg(serialPortComboBox->currentText()));

    qDebug() << ("TX-transact: Sending - ") << (payload.isEmpty() ? requestLineEdit->text() : payload);
    sthread.transaction( serialPortComboBox->currentText(),
                         timeoutSpinBox->value(),
                         ( (payload.isEmpty() ? requestLineEdit->text() : payload) ));
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


void MotorGui::showRequest(const QString &str)
{
    //qDebug() << ("Rx-showRequest!");
    trafficLabelRx->setText(tr("Traffic, transaction #%1:"
                               "\n\r-Received: %2")
                               /*"\n\r-response: %3")*/
                            .arg(++transactionCountRx)
                            .arg(str));
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


/*******************************************************************************
 * Sending Commands
 *******************************************************************************/

void MotorGui::motorDcSendReverse()
{
    //qDebug() << ("Sending DC Rev");
    QString payload = "DC,REV";
    transaction( payload );
}
void MotorGui::motorDcSendStop()
{
    //qDebug() << ("Sending DC Stop");
    QString payload = "DC,STOP";
    transaction( payload );
}
void MotorGui::motorDcSendForward()
{
    //qDebug() << ("Sending DC Fwd");
    QString payload = "DC,FWD";
    transaction( payload );
}

void MotorGui::motorSrvSendCcw()
{
    //qDebug() << ("Sending Srv Ccw");
    QString payload = "SRV,CCW";
    transaction( payload );
}
void MotorGui::motorSrvSendStop()
{
    //qDebug() << ("Sending Srv Stop");
    QString payload = "SRV,STOP";
    transaction( payload );
}
void MotorGui::motorSrvSendCw()
{
    //qDebug() << ("Sending Srv Cw");
    QString payload = "SRV,CW";
    transaction( payload );
}


