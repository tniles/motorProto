/***************************************************************************/
/***************************************************************************/
/***************************************************************************/

#ifndef MOTORGUI_H
#define MOTORGUI_H

#include <QDialog>


QT_BEGIN_NAMESPACE
class QAction;
class QDialogButtonBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QMenu;
class QMenuBar;
class QPushButton;
class QTextEdit;
QT_END_NAMESPACE


class MotorGui : public QDialog
{
    Q_OBJECT

public:
    MotorGui();

private:
    void createMenu();
    void createHorizontalGroupBox();
    void createHorizontalGroupBoxForSerial();
    void createGridGroupBox();
    void createFormGroupBox();

#define NUM_MOTOR_BUTTONS (3)
    QMenuBar *menuBar;
    QGroupBox *horizontalGroupBox;
    QGroupBox *horizontalGroupBoxForSerial;
    QPushButton *buttons[NUM_MOTOR_BUTTONS];
    QDialogButtonBox *buttonBox;

    QLabel *m_serialPortLabel = nullptr;

    QMenu *fileMenu;
    QAction *exitAction;

    QMenu *aboutMenu;
    QAction *aboutAction;
};

#endif // MOTORGUI_H
