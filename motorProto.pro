QT += core widgets gui

# requires(qtConfig(combobox))

HEADERS     = motorGui.h
SOURCES     = motorGui.cpp \
              main.cpp

# install
target.path = ./build-Qt6-Desktop
INSTALLS += target
