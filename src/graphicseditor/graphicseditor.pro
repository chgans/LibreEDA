#-------------------------------------------------
#
# Project created by QtCreator 2015-03-16T17:04:42
#
#-------------------------------------------------

QT       += widgets opengl
QT       -= gui

TARGET = graphicseditor
TEMPLATE = lib

include($$PWD/../shared.pri)

DATA_DIR = $${BASE_DATA_DIR}/sch
SETTINGS_DIR = $${BASE_SETTINGS_DIR}/sch
SAMPLE_DIR = $${BASE_SAMPLE_DIR}/sch

DEFINES += DATA_DIR=\\\"$${DATA_DIR}\\\" SETTINGS_DIR=\\\"$${SETTINGS_DIR}\\\" SAMPLE_DIR=\\\"$${SAMPLE_DIR}\\\"

SOURCES += \
    graphicsobject.cpp \
    graphicsscene.cpp \
    graphicsview.cpp \
    igraphicsitemobserver.cpp \
    igraphicsobservableitem.cpp \
    item/graphicsbezieritem.cpp \
    item/graphicslineitem.cpp \
    item/graphicsrectitem.cpp \
    tool/graphicsbeziertool.cpp \
    tool/graphicslinetool.cpp \
    tool/graphicsrecttool.cpp \
    tool/graphicsselecttool.cpp \
    graphicstool.cpp \
    graphicsgrid.cpp \
    grid/graphicscartesiangrid.cpp \
    tool/graphicsitemaligntool.cpp \
    abstractgraphicsinteractivetool.cpp \
    abstractgraphicsinserttool.cpp \
    item/graphicscircleitem.cpp \
    tool/graphicscircletool.cpp \
    item/graphicsellipseitem.cpp \
    tool/graphicsellipsetool.cpp \
    item/graphicspolygonitem.cpp \
    tool/graphicspolygontool.cpp \
    item/graphicswireitem.cpp \
    tool/graphicswiretool.cpp \
    graphicsbezierhandle.cpp \
    graphicsregularhandle.cpp \
    abstractgraphicshandle.cpp \
    widget/taskeditwidget.cpp \
    dock/taskdockwidget.cpp \
    widget/taskoptionwidget.cpp \
    widget/coordinatewidget.cpp \
    widget/wireoperationwidget.cpp \
    graphicseditorwidget.cpp

HEADERS += \
    graphicsobject.h \
    graphicsscene.h \
    graphicsview.h \
    igraphicsitemobserver.h \
    igraphicsobservableitem.h \
    item/graphicsbezieritem.h \
    item/graphicslineitem.h \
    item/graphicsrectitem.h \
    tool/graphicsbeziertool.h \
    tool/graphicslinetool.h \
    tool/graphicsrecttool.h \
    tool/graphicsselecttool.h \
    graphicstool.h \
    graphicsgrid.h \
    grid/graphicscartesiangrid.h \
    tool/graphicsitemaligntool.h \
    abstractgraphicsinteractivetool.h \
    abstractgraphicsinserttool.h \
    item/graphicscircleitem.h \
    tool/graphicscircletool.h \
    item/graphicsellipseitem.h \
    tool/graphicsellipsetool.h \
    item/graphicspolygonitem.h \
    graphicsobject_p.h \
    tool/graphicspolygontool.h \
    item/graphicswireitem.h \
    tool/graphicswiretool.h \
    graphicsbezierhandle.h \
    graphicsregularhandle.h \
    abstractgraphicshandle.h \
    widget/taskeditwidget.h \
    dock/taskdockwidget.h \
    widget/taskoptionwidget.h \
    widget/coordinatewidget.h \
    widget/wireoperationwidget.h \
    graphicseditorwidget.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS +=

OTHER_FILES += \
    tool/graphicsarctool.svg \
    tool/graphicsbeziercurvetool.svg \
    tool/graphicsbeziersplinetool.svg \
    tool/graphicsbeziertool.svg \
    tool/graphicscircletool.svg \
    tool/graphicsellipsetool.svg \
    tool/graphicslinetool.svg \
    tool/graphicspolygontool.svg \
    tool/graphicspolylinetool.svg \
    tool/graphicsrecttool.svg

RESOURCES += graphicseditor.qrc

