QT += core widgets gui serialport

requires(qtConfig(combobox))

HEADERS     +=  motorGui.h \
                receiverThread.h \
                senderThread.h
SOURCES     +=  main.cpp \
                motorGui.cpp \
                receiverThread.cpp \
                senderThread.cpp

# install
target.path = ./build-Qt6-Desktop
INSTALLS += target
