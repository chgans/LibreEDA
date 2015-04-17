#-------------------------------------------------
#
# Project created by QtCreator 2015-03-17T16:26:52
#
#-------------------------------------------------

QT       += widgets

QT       -= gui

TARGET = core
TEMPLATE = lib
CONFIG += staticlib

SOURCES += core.cpp \
    abstracteditor.cpp

HEADERS += core.h \
    constants.h \
    abstracteditor.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES += \
    TODO.md
