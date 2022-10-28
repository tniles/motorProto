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
    serialPortLabel     = new QLabel(tr("Serial port:"));
    serialPortComboBox  = new QComboBox;
    waitResponseLabel   = new QLabel(tr("Wait response (ms):"));
    waitResponseSpinBox = new QSpinBox;
    requestLabel        = new QLabel(tr("Request:"));
    requestLineEdit     = new QLineEdit(tr("Who are you?"));
    trafficLabel        = new QLabel(tr("No traffic."));
    statusLabel         = new QLabel(tr("Status: Not running."));
    runButton           = new QPushButton(tr("Start"));

    createMenu();
    createHorizontalGroupBoxForSerial();
    createHorizontalGroupBox();

    /*
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                     | QDialogButtonBox::Cancel);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    */

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setMenuBar(menuBar);
    mainLayout->addWidget(horizontalGroupBoxForSerial);
    mainLayout->addWidget(horizontalGroupBox);
    //mainLayout->addWidget(buttonBox);
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


void MotorGui::createHorizontalGroupBox()
{
    horizontalGroupBox = new QGroupBox(tr("Commands"));

    QHBoxLayout *layout = new QHBoxLayout;

    int ii = 0;
    buttons[ii++] = new QPushButton(tr("Reverse"));
    buttons[ii++] = new QPushButton(tr("Stop"));
    buttons[ii++] = new QPushButton(tr("Forward"));
    for (ii = 0; ii < NUM_MOTOR_BUTTONS; ii++) {
        layout->addWidget(buttons[ii]);
    }

    horizontalGroupBox->setLayout(layout);
}


#define USE_GRIDLAYOUT
void MotorGui::createHorizontalGroupBoxForSerial()
{
    horizontalGroupBoxForSerial = new QGroupBox(tr("Serial Port"));

#ifndef USE_GRIDLAYOUT
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(serialPortLabel);
    layout->addWidget(serialPortComboBox);
    layout->addWidget(waitResponseLabel);
    layout->addWidget(waitResponseSpinBox);
    layout->addWidget(runButton);
    layout->addWidget(requestLabel);
    layout->addWidget(requestLineEdit);
    layout->addWidget(trafficLabel);
    layout->addWidget(statusLabel);
#else
    auto *layout = new QGridLayout;
    layout->addWidget(serialPortLabel);
    layout->addWidget(serialPortComboBox,     0, 1);
    layout->addWidget(waitResponseLabel,      1, 0);
    layout->addWidget(waitResponseSpinBox,    1, 1);
    layout->addWidget(runButton,              4, 2, 2, 1);
    layout->addWidget(requestLabel,           2, 0);
    layout->addWidget(requestLineEdit,        2, 1, 1, 3);
    layout->addWidget(trafficLabel,           3, 0, 1, 4);
    layout->addWidget(statusLabel,            4, 0, 1, 5);
#endif /* USE_GRIDLAYOUT */

    /* Configure Spinbox, ms */
    waitResponseSpinBox->setRange(1, 5000);
    waitResponseSpinBox->setValue(1000);

    horizontalGroupBoxForSerial->setLayout(layout);

    serialPortComboBox->setFocus();

    connect(runButton, &QPushButton::clicked, this, &MotorGui::transaction);
    connect(&sthread, &SenderThread::response, this, &MotorGui::showResponse);
    connect(&sthread, &SenderThread::error,    this, &MotorGui::processError);
    connect(&sthread, &SenderThread::timeout,  this, &MotorGui::processTimeout);
}


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


