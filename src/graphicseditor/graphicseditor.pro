#-------------------------------------------------
#
# Project created by QtCreator 2015-03-16T17:04:42
#
#-------------------------------------------------

QT       += widgets

QT       -= gui

TARGET = graphicseditor
TEMPLATE = lib
CONFIG += staticlib

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
    tool/graphicsselecttool.cpp

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
    tool/graphicsselecttool.h
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
