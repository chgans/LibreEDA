#-------------------------------------------------
#
# Project created by QtCreator 2015-04-23T12:12:11
#
#-------------------------------------------------

QT       += widgets

TARGET = filesystemnavigationview
TEMPLATE = lib

include($$PWD/../../shared.pri)

SOURCES += \
    filesystemnavigationviewfactory.cpp \
    filesystemnavigationwidget.cpp

HEADERS += \
    filesystemnavigationviewfactory.h \
    filesystemnavigationwidget.h

OTHER_FILES += \
    filesystemnavigationview.pri

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
