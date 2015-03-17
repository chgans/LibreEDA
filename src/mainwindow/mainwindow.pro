#-------------------------------------------------
#
# Project created by QtCreator 2015-03-16T17:06:49
#
#-------------------------------------------------

QT       += core gui opengl
CONFIG   += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = libreeda
TEMPLATE = app

INCLUDEPATH += $$PWD/..

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../core/release/ -lcore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../core/debug/ -lcore
else:unix: LIBS += -L$$OUT_PWD/../core/ -lcore

DEPENDPATH += $$PWD/../core

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../core/release/libcore.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../core/debug/libcore.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../core/release/core.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../core/debug/core.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../core/libcore.a

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../graphicseditor/release/ -lgraphicseditor
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../graphicseditor/debug/ -lgraphicseditor
else:unix: LIBS += -L$$OUT_PWD/../graphicseditor/ -lgraphicseditor

DEPENDPATH += $$PWD/../graphicseditor

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../graphicseditor/release/libgraphicseditor.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../graphicseditor/debug/libgraphicseditor.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../graphicseditor/release/graphicseditor.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../graphicseditor/debug/graphicseditor.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../graphicseditor/libgraphicseditor.a

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../logviewer/release/ -llogviewer
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../logviewer/debug/ -llogviewer
else:unix: LIBS += -L$$OUT_PWD/../logviewer/ -llogviewer

DEPENDPATH += $$PWD/../logviewer

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../logviewer/release/liblogviewer.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../logviewer/debug/liblogviewer.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../logviewer/release/logviewer.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../logviewer/debug/logviewer.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../logviewer/liblogviewer.a
