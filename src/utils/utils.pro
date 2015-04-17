#-------------------------------------------------
#
# Project created by QtCreator 2015-04-17T17:09:53
#
#-------------------------------------------------

QT       += widgets
QT       -= gui
CONFIG += c++11

TARGET = utils
TEMPLATE = lib

include($$PWD/../shared.pri)

SOURCES += utils.cpp \
    widgets/pensettingswidget.cpp \
    widgets/pencapstylecombobox.cpp \
    widgets/penstylecombobox.cpp \
    widgets/penjoinstylecombobox.cpp \
    widgets/penwidthcombobox.cpp \
    widgets/colorpreview.cpp \
    widgets/colorwheel.cpp \
    widgets/gradientslider.cpp \
    widgets/hueslider.cpp \
    widgets/colorwidget.cpp

HEADERS += utils.h \
    widgets/pensettingswidget.h \
    widgets/pencapstylecombobox.h \
    widgets/penstylecombobox.h \
    widgets/penjoinstylecombobox.h \
    widgets/penwidthcombobox.h \
    widgets/colorpreview.h \
    widgets/colorwheel.h \
    widgets/gradientslider.h \
    widgets/hueslider.h \
    widgets/paintborder.h \
    widgets/colorwidget.h

