/****************************************************************************

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

****************************************************************************/

#ifndef MOTORGUI_H
#define MOTORGUI_H

#include <QDialog>

#include "receiverThread.h"
#include "senderThread.h"


QT_BEGIN_NAMESPACE

class QAction;
class QComboBox;
class QDialogButtonBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QMenu;
class QMenuBar;
class QPushButton;
class QSpinBox;
class QTextEdit;

QT_END_NAMESPACE


class MotorGui : public QDialog
{
    Q_OBJECT

public:
    MotorGui();

private slots:
    /* For Tx */
    void transaction();
    void showResponse(const QString &str);
    void processError(const QString &str);
    void processTimeout(const QString &str);
    /* For Rx */
    void startReceiver();
    void showRequest(const QString &s);
    void processErrorRx(const QString &str);
    void processTimeoutRx(const QString &str);
    void setControlsEnabledRx();

private:
    void createMenu();
    void createHBoxSerialPortSelect();
    void createHBoxSerialRx();
    void createHBoxSerialTx();
    void setControlsEnabled(bool enable);
    void createHBoxDcMotorCmds();
    void createHBoxServoMotorCmds();

#define NUM_MOTOR_BUTTONS (3)
    QMenuBar            *menuBar;
    QGroupBox           *hBoxSerialPortSelect;
    QGroupBox           *hBoxSerialRx;
    QGroupBox           *hBoxSerialTx;
    QGroupBox           *hBoxDcMotorCmds;
    QGroupBox           *hBoxServoMotorCmds;
    QPushButton         *buttons[NUM_MOTOR_BUTTONS];
    QDialogButtonBox    *buttonBox;

    QMenu   *fileMenu;
    QAction *exitAction;
    QMenu   *aboutMenu;
    QAction *aboutAction;

    /* Common */
    QLabel      *serialPortLabel        = nullptr;
    QComboBox   *serialPortComboBox     = nullptr;
    QLabel      *timeoutLabel           = nullptr;
    QSpinBox    *timeoutSpinBox         = nullptr;

    /* For Tx */
    int         transactionCount        = 0;
    QLabel      *requestLabel           = nullptr;
    QLineEdit   *requestLineEdit        = nullptr;
    QLabel      *trafficLabel           = nullptr;
    QLabel      *statusLabel            = nullptr;
    QPushButton *runButton              = nullptr;

    /* For Rx */
    int         transactionCountRx      = 0;
    QLabel      *responseLabelRx        = nullptr;
    QLineEdit   *responseLineEditRx     = nullptr;
    QLabel      *trafficLabelRx         = nullptr;
    QLabel      *statusLabelRx          = nullptr;
    QPushButton *runButtonRx            = nullptr;

    SenderThread    sthread;
    ReceiverThread  rthread;
};


#endif // MOTORGUI_H
