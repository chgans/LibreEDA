#-------------------------------------------------
#
# Project created by QtCreator 2015-03-16T17:05:48
#
#-------------------------------------------------

QT       += widgets
QT       -= gui

TARGET = logviewer
TEMPLATE = lib

include($$PWD/../../shared.pri)

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

OTHER_FILES += \
    logviewer.pri


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../core/release/ -lcore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../core/debug/ -lcore
else:unix: LIBS += -L$$OUT_PWD/../../core/ -lcore

INCLUDEPATH += $$PWD/../../core
DEPENDPATH += $$PWD/../../core

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../utils/release/ -lutils
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../utils/debug/ -lutils
else:unix: LIBS += -L$$OUT_PWD/../../utils/ -lutils

INCLUDEPATH += $$PWD/../../utils
DEPENDPATH += $$PWD/../../utils
