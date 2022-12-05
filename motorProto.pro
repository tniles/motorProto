QT += core widgets gui serialport

requires(qtConfig(combobox))

!versionAtLeast(QT_VERSION, 6.2.0) {
    message("Cannot use Qt $${QT_VERSION}")
    error("Cannot use this version of Qt. Please use Qt 6.2.0 or newer.")
}

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
