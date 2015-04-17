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
    abstracteditor.cpp

HEADERS += core.h \
    constants.h \
    abstracteditor.h

DISTFILES += \
    TODO.md
