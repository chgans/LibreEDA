#-------------------------------------------------
#
# Project created by QtCreator 2015-03-16T17:06:49
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = libreeda
TEMPLATE = app

include($$PWD/../shared.pri)

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../core/release/ -lcore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../core/debug/ -lcore
else:unix: LIBS += -L$$OUT_PWD/../core/ -lcore

INCLUDEPATH += $$PWD/../core
DEPENDPATH += $$PWD/../core

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../utils/release/ -lutils
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../utils/debug/ -lutils
else:unix: LIBS += -L$$OUT_PWD/../utils/ -lutils

INCLUDEPATH += $$PWD/../utils
DEPENDPATH += $$PWD/../utils

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../plugins/scheditor/release/ -lscheditor
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../plugins/scheditor/debug/ -lscheditor
else:unix: LIBS += -L$$OUT_PWD/../plugins/scheditor/ -lscheditor

INCLUDEPATH += $$PWD/../plugins/scheditor
DEPENDPATH += $$PWD/../plugins/scheditor

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../plugins/filesystemnavigationview/release/ -lfilesystemnavigationview
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../plugins/filesystemnavigationview/debug/ -lfilesystemnavigationview
else:unix: LIBS += -L$$OUT_PWD/../plugins/filesystemnavigationview/ -lfilesystemnavigationview

INCLUDEPATH += $$PWD/../plugins/filesystemnavigationview
DEPENDPATH += $$PWD/../plugins/filesystemnavigationview

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../plugins/logviewer/release/ -llogviewer
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../plugins/logviewer/debug/ -llogviewer
else:unix: LIBS += -L$$OUT_PWD/../plugins/logviewer/ -llogviewer

INCLUDEPATH += $$PWD/../plugins/logviewer
DEPENDPATH += $$PWD/../plugins/logviewer

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../plugins/pcbeditor/release/ -lpcbeditor
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../plugins/pcbeditor/debug/ -lpcbeditor
else:unix: LIBS += -L$$OUT_PWD/../plugins/pcbeditor/ -lpcbeditor

INCLUDEPATH += $$PWD/../plugins/pcbeditor
DEPENDPATH += $$PWD/../plugins/pcbeditor

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../utils/release/ -lutils
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../utils/debug/ -lutils
else:unix: LIBS += -L$$OUT_PWD/../utils/ -lutils

INCLUDEPATH += $$PWD/../utils
DEPENDPATH += $$PWD/../utils
