/***************************************************************************/
/***************************************************************************/
/***************************************************************************/

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
    void createHorizontalGroupBoxCmds();
    void createHorizontalGroupBoxSerialRx();
    void createHorizontalGroupBoxSerialTx();
    //void createGridGroupBox();
    //void createFormGroupBox();
    void setControlsEnabled(bool enable);

#define NUM_MOTOR_BUTTONS (3)
    QMenuBar            *menuBar;
    QGroupBox           *horizontalGroupBoxCmds;
    QGroupBox           *horizontalGroupBoxSerialRx;
    QGroupBox           *horizontalGroupBoxSerialTx;
    QPushButton         *buttons[NUM_MOTOR_BUTTONS];
    QDialogButtonBox    *buttonBox;

    QMenu   *fileMenu;
    QAction *exitAction;
    QMenu   *aboutMenu;
    QAction *aboutAction;

    /* For Tx */
    int         transactionCount        = 0;
    QLabel      *serialPortLabel        = nullptr;
    QComboBox   *serialPortComboBox     = nullptr;
    QLabel      *waitResponseLabel      = nullptr;
    QSpinBox    *waitResponseSpinBox    = nullptr;
    QLabel      *requestLabel           = nullptr;
    QLineEdit   *requestLineEdit        = nullptr;
    QLabel      *trafficLabel           = nullptr;
    QLabel      *statusLabel            = nullptr;
    QPushButton *runButton              = nullptr;

    /* For Rx */
    int         transactionCountRx      = 0;
    QLabel      *serialPortLabelRx      = nullptr;
    QComboBox   *serialPortComboBoxRx   = nullptr;
    QLabel      *waitRequestLabelRx     = nullptr;
    QSpinBox    *waitRequestSpinBoxRx   = nullptr;
    QLabel      *responseLabelRx        = nullptr;
    QLineEdit   *responseLineEditRx     = nullptr;
    QLabel      *trafficLabelRx         = nullptr;
    QLabel      *statusLabelRx          = nullptr;
    QPushButton *runButtonRx            = nullptr;

    SenderThread    sthread;
    ReceiverThread  rthread;
};


#endif // MOTORGUI_H
