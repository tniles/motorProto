/***************************************************************************/
/***************************************************************************/
/***************************************************************************/

#include <QtWidgets>
#include "motorGui.h"


MotorGui::MotorGui()
{
    createMenu();
    createHorizontalGroupBox();

    /*
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                     | QDialogButtonBox::Cancel);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    */

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setMenuBar(menuBar);
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

    connect(exitAction, &QAction::triggered, this, &QDialog::accept);
}


void MotorGui::createHorizontalGroupBox()
{
    horizontalGroupBox = new QGroupBox(tr("Commands"));
    QHBoxLayout *layout = new QHBoxLayout;

    int ii = 0;
    buttons[ii++] = new QPushButton(tr("Reverse"));
    buttons[ii++] = new QPushButton(tr("Stop"));
    buttons[ii++] = new QPushButton(tr("Forward"));
    for (ii = 0; ii < NUM_BUTTONS; ii++) {
        layout->addWidget(buttons[ii]);
    }

    horizontalGroupBox->setLayout(layout);
}


