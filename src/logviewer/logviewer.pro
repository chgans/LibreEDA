#-------------------------------------------------
#
# Project created by QtCreator 2015-03-16T17:05:48
#
#-------------------------------------------------

QT       += widgets
QT       -= gui

TARGET = logviewer
TEMPLATE = lib

include($$PWD/../shared.pri)

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

FORMS += \
    logwidget.ui
