/***************************************************************************/
/***************************************************************************/
/***************************************************************************/

#include <QtWidgets>
#include "motorGui.h"


MotorGui::MotorGui()
{
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


void MotorGui::createHorizontalGroupBoxForSerial()
{
    horizontalGroupBoxForSerial = new QGroupBox(tr("Serial Port"));
    QHBoxLayout *layout = new QHBoxLayout;

    m_serialPortLabel = new QLabel(tr("Serial port:"));
    layout->addWidget(m_serialPortLabel);

    horizontalGroupBoxForSerial->setLayout(layout);
}


