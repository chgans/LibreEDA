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
CONFIG += staticlib

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

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../core/release/ -lcore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../core/debug/ -lcore
else:unix: LIBS += -L$$OUT_PWD/../core/ -lcore

INCLUDEPATH += $$PWD/../core
DEPENDPATH += $$PWD/../core

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../core/release/libcore.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../core/debug/libcore.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../core/release/core.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../core/debug/core.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../core/libcore.a
