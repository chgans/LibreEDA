#-------------------------------------------------
#
# Project created by QtCreator 2015-03-16T17:06:49
#
#-------------------------------------------------

QT       += core gui opengl
CONFIG   += c++11

TARGET = libreeda
TEMPLATE = app
LIBS += -L$$OUT_PWD/.. -lgraphicseditor -lpcbeditor -llogviewer -lutils -lcore
include($$PWD/../shared.pri)

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    +=
