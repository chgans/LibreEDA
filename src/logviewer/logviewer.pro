#-------------------------------------------------
#
# Project created by QtCreator 2015-03-16T17:05:48
#
#-------------------------------------------------

QT       += widgets

QT       -= gui

TARGET = logviewer
TEMPLATE = lib

DEFINES += LOGVIEWER_LIBRARY

SOURCES += logviewer.cpp

HEADERS += logviewer.h\
        logviewer_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
