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
    navigationview/navigationdockwidget.cpp \
    json.cpp

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
    navigationview/navigationdockwidget.h \
    json.h

DISTFILES += \
    TODO.md

OTHER_FILES += \
    core.pri

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../utils/release/ -lutils
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../utils/debug/ -lutils
else:unix: LIBS += -L$$OUT_PWD/../utils/ -lutils

INCLUDEPATH += $$PWD/../utils
DEPENDPATH += $$PWD/../utils
