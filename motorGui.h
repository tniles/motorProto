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
    void createGridGroupBox();
    void createFormGroupBox();

#define NUM_BUTTONS (3)
    QMenuBar *menuBar;
    QGroupBox *horizontalGroupBox;
    QPushButton *buttons[NUM_BUTTONS];
    QDialogButtonBox *buttonBox;

    QMenu *fileMenu;
    QAction *exitAction;
};

#endif // MOTORGUI_H
