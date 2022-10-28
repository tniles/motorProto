/***************************************************************************/
/***************************************************************************/
/***************************************************************************/

#ifndef MOTORGUI_H
#define MOTORGUI_H

#include <QDialog>

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
    void transaction();
    void showResponse(const QString &str);
    void processError(const QString &str);
    void processTimeout(const QString &str);

private:
    void createMenu();
    void createHorizontalGroupBox();
    void createHorizontalGroupBoxForSerial();
    void createGridGroupBox();
    void createFormGroupBox();
    void setControlsEnabled(bool enable);

#define NUM_MOTOR_BUTTONS (3)
    QMenuBar            *menuBar;
    QGroupBox           *horizontalGroupBox;
    QGroupBox           *horizontalGroupBoxForSerial;
    QPushButton         *buttons[NUM_MOTOR_BUTTONS];
    QDialogButtonBox    *buttonBox;

    QMenu   *fileMenu;
    QAction *exitAction;
    QMenu   *aboutMenu;
    QAction *aboutAction;

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

    SenderThread sthread;
};


#endif // MOTORGUI_H
