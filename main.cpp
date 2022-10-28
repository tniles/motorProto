/***************************************************************************/
/***************************************************************************/
/***************************************************************************/

#include <QApplication>
#include <QDebug>
//    qInfo() << "reached step 1";

#include "motorGui.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MotorGui motorGui;
    motorGui.show();
    return app.exec();
}

