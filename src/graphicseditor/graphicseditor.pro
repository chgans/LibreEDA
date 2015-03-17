#-------------------------------------------------
#
# Project created by QtCreator 2015-03-16T17:04:42
#
#-------------------------------------------------

QT       += widgets opengl
QT       -= gui
CONFIG   += c++11

TARGET = graphicseditor
TEMPLATE = lib
CONFIG += staticlib
INCLUDEPATH += $$PWD/..

SOURCES += graphicseditor.cpp \
    graphicshandle.cpp \
    graphicsobject.cpp \
    graphicspathpoint.cpp \
    graphicsscene.cpp \
    graphicsview.cpp \
    igraphicsitemobserver.cpp \
    igraphicsobservableitem.cpp \
    item/graphicsbezieritem.cpp \
    item/graphicslineitem.cpp \
    item/graphicsrectitem.cpp \
    tool/graphicsbeziertool.cpp \
    tool/graphicslinetool.cpp \
    tool/graphicslinetooldialog.cpp \
    tool/graphicsrecttool.cpp \
    tool/graphicsselecttool.cpp \
    graphicstool.cpp

HEADERS += graphicseditor.h \
    graphicshandle.h \
    graphicsobject.h \
    graphicspathpoint.h \
    graphicsscene.h \
    graphicsview.h \
    igraphicsitemobserver.h \
    igraphicsobservableitem.h \
    item/graphicsbezieritem.h \
    item/graphicslineitem.h \
    item/graphicsrectitem.h \
    tool/graphicsbeziertool.h \
    tool/graphicslinetool.h \
    tool/graphicslinetooldialog.h \
    tool/graphicsrecttool.h \
    tool/graphicsselecttool.h \
    graphicstool.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    tool/graphicslinetooldialog.ui

OTHER_FILES += \
    tool/graphicsbeziertool.svg \
    tool/graphicslinetool.svg \
    tool/graphicsrecttool.svg

RESOURCES += \
    graphicseditor.qrc

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
