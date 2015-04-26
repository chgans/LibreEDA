#-------------------------------------------------
#
# Project created by QtCreator 2014-05-15T11:03:01
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = pcbeditor
TEMPLATE = lib

include($$PWD/../../shared.pri)

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
    pcbdocument.cpp \
    views/layoutview.cpp

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
    pcbdocument.h \
    views/layoutview.h

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

OTHER_FILES += \
    pcbeditor.pri

RESOURCES += \
    pcbeditor.qrc

DISTFILES += \
    notes/board-shape.txt \
    notes/layers.txt \
    notes/TODO.txt


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
