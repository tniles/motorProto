/***************************************************************************/
/***************************************************************************/
/***************************************************************************/

#include <QtWidgets>
#include <QComboBox>
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
    /* For Tx */
    serialPortLabel     = new QLabel(tr("Select port:"));
    serialPortComboBox  = new QComboBox;
    waitResponseLabel   = new QLabel(tr("Wait response (ms):"));
    waitResponseSpinBox = new QSpinBox;
    requestLabel        = new QLabel(tr("Request:"));
    requestLineEdit     = new QLineEdit(tr("Enter request here..."));
    trafficLabel        = new QLabel(tr("No traffic."));
    statusLabel         = new QLabel(tr("Status: Not running."));
    runButton           = new QPushButton(tr("Start"));

    /* For Rx */
    serialPortLabelRx       = new QLabel(tr("Select port:"));
    serialPortComboBoxRx    = new QComboBox;
    waitRequestLabelRx      = new QLabel(tr("Wait request (ms):"));
    waitRequestSpinBoxRx    = new QSpinBox;
    responseLabelRx         = new QLabel(tr("Response:"));
    responseLineEditRx      = new QLineEdit(tr("Standing by to receive..."));
    trafficLabelRx          = new QLabel(tr("No traffic."));
    statusLabelRx           = new QLabel(tr("Status: Not running."));
    runButtonRx             = new QPushButton(tr("Start"));
 
    /* Go gather port info for currently connected devices.
     * Need to add functionality to detect dynamic changes since
     * as written this is done only once on app startup. */
    const auto sInfo = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : sInfo)
    {
        serialPortComboBox->addItem(info.portName());
        serialPortComboBoxRx->addItem(info.portName());
    }

    createMenu();
    createHorizontalGroupBoxSerialTx();
    createHorizontalGroupBoxSerialRx();
    createHorizontalGroupBoxCmds();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setMenuBar(menuBar);
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


void MotorGui::createHorizontalGroupBoxSerialTx()
{
    horizontalGroupBoxSerialTx = new QGroupBox(tr("Serial Port Tx"));

    auto *layout = new QGridLayout;
    layout->addWidget(serialPortLabel);
    layout->addWidget(serialPortComboBox,   0, 1);
    layout->addWidget(waitResponseLabel,    1, 0);
    layout->addWidget(waitResponseSpinBox,  1, 1);
    layout->addWidget(runButton,            0, 2, 2, 1);
    layout->addWidget(requestLabel,         2, 0);
    layout->addWidget(requestLineEdit,      2, 1, 1, 3);
    layout->addWidget(trafficLabel,         3, 0, 1, 4);
    layout->addWidget(statusLabel,          4, 0, 1, 5);

    /* Configure Spinbox, ms */
    waitResponseSpinBox->setRange(1, 5000);
    waitResponseSpinBox->setValue(1000);

    horizontalGroupBoxSerialTx->setLayout(layout);

    serialPortComboBox->setFocus();

    connect(runButton, &QPushButton::clicked,   this, &MotorGui::transaction);
    connect(&sthread,  &SenderThread::response, this, &MotorGui::showResponse);
    connect(&sthread,  &SenderThread::error,    this, &MotorGui::processError);
    connect(&sthread,  &SenderThread::timeout,  this, &MotorGui::processTimeout);
}


void MotorGui::createHorizontalGroupBoxSerialRx()
{
    horizontalGroupBoxSerialRx = new QGroupBox(tr("Serial Port Rx"));

    auto *layout = new QGridLayout;
    layout->addWidget(serialPortLabelRx,    0, 0);
    layout->addWidget(serialPortComboBoxRx, 0, 1);
    layout->addWidget(waitRequestLabelRx,   1, 0);
    layout->addWidget(waitRequestSpinBoxRx, 1, 1);
    layout->addWidget(runButtonRx,          0, 2, 2, 1);
    layout->addWidget(responseLabelRx,      2, 0);
    layout->addWidget(responseLineEditRx,   2, 1, 1, 3);
    layout->addWidget(trafficLabelRx,       3, 0, 1, 4);
    layout->addWidget(statusLabelRx,        4, 0, 1, 5);

    /* Configure Spinbox, ms */
    waitRequestSpinBoxRx->setRange(1, 5000);
    waitRequestSpinBoxRx->setValue(1000);

    horizontalGroupBoxSerialRx->setLayout(layout);

    serialPortComboBox->setFocus();
    connect(runButtonRx, &QPushButton::clicked,     this, &MotorGui::startReceiver);
    connect(&rthread,    &ReceiverThread::request,  this, &MotorGui::showRequest);
    connect(&rthread,    &ReceiverThread::error,    this, &MotorGui::processErrorRx);
    connect(&rthread,    &ReceiverThread::timeout,  this, &MotorGui::processTimeoutRx);

    connect(waitRequestSpinBoxRx, &QSpinBox::textChanged,  this, &MotorGui::setControlsEnabledRx);
    connect(responseLineEditRx,   &QLineEdit::textChanged, this, &MotorGui::setControlsEnabledRx);
    connect(serialPortComboBoxRx, &QComboBox::currentIndexChanged, \
            this, &MotorGui::setControlsEnabledRx);
}


/*******************************************************************************
 * Tx Section
 ******************************************************************************/
void MotorGui::transaction()
{
    setControlsEnabled(false);

    statusLabel->setText(tr("Status: Running, connected to port %1.")
                           .arg(serialPortComboBox->currentText()));

    sthread.transaction(serialPortComboBox->currentText(),
                         waitResponseSpinBox->value(),
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
}


void MotorGui::processError(const QString &str)
{
    setControlsEnabled(true);
    statusLabel->setText(tr("Status: Not running, %1.").arg(str));
    trafficLabel->setText(tr("No traffic."));
}


void MotorGui::processTimeout(const QString &str)
{
    setControlsEnabled(true);
    statusLabel->setText(tr("Status: Running, %1.").arg(str));
    trafficLabel->setText(tr("No traffic."));
}


void MotorGui::setControlsEnabled(bool enable)
{
    runButton->setEnabled(enable);
    serialPortComboBox->setEnabled(enable);
    waitResponseSpinBox->setEnabled(enable);
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
    runButtonRx->setEnabled(false);
    statusLabel->setText(tr("Status: Running, connected to port %1.")
                           .arg(serialPortComboBox->currentText()));
    rthread.startReceiver(serialPortComboBoxRx->currentText(),
                        waitRequestSpinBoxRx->value(),
                        responseLineEditRx->text());
}


void MotorGui::showRequest(const QString &s)
{
    trafficLabelRx->setText(tr("Traffic, transaction #%1:"
                               "\n\r-Received: %2")
                               /*"\n\r-response: %3")*/
                            .arg(++transactionCountRx)
                            .arg(s));
                            /*.arg(responseLineEditRx->text()));*/
}


void MotorGui::processErrorRx(const QString &str)
{
    setControlsEnabledRx();
    statusLabelRx->setText(tr("Status: Not running, %1.").arg(str));
    trafficLabelRx->setText(tr("No traffic."));
}


void MotorGui::processTimeoutRx(const QString &str)
{
    setControlsEnabledRx();
    statusLabelRx->setText(tr("Status: Running, %1.").arg(str));
    trafficLabelRx->setText(tr("No traffic."));
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

