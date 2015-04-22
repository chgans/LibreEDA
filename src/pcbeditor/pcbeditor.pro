#-------------------------------------------------
#
# Project created by QtCreator 2014-05-15T11:03:01
#
#-------------------------------------------------

QT       += core gui widgets
CONFIG += c++11

TARGET = pcbeditor
TEMPLATE = lib

include($$PWD/../shared.pri)

DATA_DIR = $${BASE_DATA_DIR}/pcb
SETTINGS_DIR = $${BASE_SETTINGS_DIR}/pcb
SAMPLE_DIR = $${BASE_SAMPLE_DIR}/pcb

DEFINES += DATA_DIR=\\\"$${DATA_DIR}\\\" SETTINGS_DIR=\\\"$${SETTINGS_DIR}\\\" SAMPLE_DIR=\\\"$${SAMPLE_DIR}\\\"

SOURCES += \
    scene.cpp \
    pcbeditorwidget.cpp \
    pcbpalette.cpp \
    pcbpalettesettingsdialog.cpp \
    pcbpalettemanager.cpp \
    pcbphysicallayer.cpp \
    pcbphysicalboard.cpp \
    designlayer.cpp \
    primitive.cpp \
    designlayerset.cpp \
    layerbar.cpp \
    designlayermanager.cpp \
    views/detailview.cpp \
    views/objectpreview.cpp \
    views/overview.cpp \
    views/mainview.cpp \
    insight/insightdockwidget.cpp \
    insight/insightlenswidget.cpp \
    insight/insightheadsupwidget.cpp \
    insight/insightconnectivitywidget.cpp \
    insight/insightpicklistwidget.cpp \
    colorprofileeditor.cpp \
    items/graphicsitem.cpp \
    items/graphicsrect.cpp \
    items/graphicsline.cpp \
    pcbeditorfactory.cpp \
    pcbeditor.cpp \
    pcbdocument.cpp

HEADERS  += \
    scene.h \
    pcbeditorwidget.h \
    pcbpalette.h \
    pcbpalettesettingsdialog.h \
    pcbpalettemanager.h \
    pcbphysicallayer.h \
    pcbphysicalboard.h \
    designlayer.h \
    primitive.h \
    designlayerset.h \
    layerbar.h \
    designlayermanager.h \
    views/detailview.h \
    views/objectpreview.h \
    views/overview.h \
    views/mainview.h \
    insight/insightdockwidget.h \
    insight/insightlenswidget.h \
    insight/insightheadsupwidget.h \
    insight/insightconnectivitywidget.h \
    insight/insightpicklistwidget.h \
    colorprofileeditor.h \
    items/graphicsitem.h \
    items/graphicsrect.h \
    items/graphicsline.h \
    pcbeditorfactory.h \
    pcbeditor.h \
    pcbeditorconstants.h \
    pcbdocument.h

FORMS    += \
    pcbpalettesettingsdialog.ui \
    colorprofileeditor.ui

OTHER_FILES += \
    icons/alpha-background.png \
    icons/chip-package.png \
    icons/PCB-icon.png \
    settings/AltiumClassic.LedaPcbPalette \
    settings/AltiumDefault.LedaPcbPalette \
    settings/AltiumDxp2004.LedaPcbPalette \
    settings/LedaDefault.LedaPcbPalette

RESOURCES += \
    pcbeditor.qrc

DISTFILES += \
    notes/board-shape.txt \
    notes/layers.txt \
    notes/TODO.txt
