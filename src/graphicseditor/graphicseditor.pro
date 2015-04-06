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
    widget/pensettingswidget.cpp \
    widget/pencapstylecombobox.cpp \
    widget/penstylecombobox.cpp \
    widget/penjoinstylecombobox.cpp \
    widget/penwidthcombobox.cpp \
    widget/taskeditwidget.cpp \
    dock/taskdockwidget.cpp \
    widget/taskoptionwidget.cpp \
    widget/coordinatewidget.cpp \
    widget/wireoperationwidget.cpp

HEADERS += graphicseditor.h \
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
    widget/pensettingswidget.h \
    widget/pencapstylecombobox.h \
    widget/penstylecombobox.h \
    widget/penjoinstylecombobox.h \
    widget/penwidthcombobox.h \
    widget/taskeditwidget.h \
    dock/taskdockwidget.h \
    widget/taskoptionwidget.h \
    widget/coordinatewidget.h \
    widget/wireoperationwidget.h

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
