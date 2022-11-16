/*******************************************************************************

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

*******************************************************************************/

#include <QApplication>
#include <QDebug>                   // qInfo() << "reached step 1";

#include "motorGui.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MotorGui motorGui;
    motorGui.show();
    return app.exec();
}

