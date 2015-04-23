#-------------------------------------------------
#
# Project created by QtCreator 2015-03-17T16:26:52
#
#-------------------------------------------------

QT       += widgets
QT       -= gui

TARGET = core
TEMPLATE = lib

include($$PWD/../shared.pri)

SOURCES += core.cpp \
    abstracteditor.cpp \
    actionmanager/actionmanager.cpp \
    editormanager/editormanager.cpp \
    editormanager/ieditor.cpp \
    editormanager/ieditorfactory.cpp \
    editormanager/idocument.cpp \
    editormanager/editorview.cpp \
    navigationview/inavigationviewfactory.cpp \
    navigationview/navigationdockwidget.cpp

HEADERS += core.h \
    constants.h \
    abstracteditor.h \
    actionmanager/actionmanager.h \
    editormanager/editormanager.h \
    editormanager/ieditor.h \
    editormanager/ieditorfactory.h \
    editormanager/idocument.h \
    editormanager/editorview.h \
    navigationview/inavigationviewfactory.h \
    navigationview/navigationdockwidget.h

DISTFILES += \
    TODO.md
