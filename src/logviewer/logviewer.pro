#-------------------------------------------------
#
# Project created by QtCreator 2015-03-16T17:05:48
#
#-------------------------------------------------

QT       += widgets
QT       -= gui
CONFIG   += c++11

TARGET = logviewer
TEMPLATE = lib

DEFINES += LOGVIEWER_LIBRARY

SOURCES += logviewer.cpp \
    logmessage.cpp \
    logmodel.cpp \
    logwidget.cpp \
    logsortfilterproxymodel.cpp

HEADERS += logviewer.h\
        logviewer_global.h \
    logmessage.h \
    logmodel.h \
    logwidget.h \
    logsortfilterproxymodel.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    logwidget.ui
