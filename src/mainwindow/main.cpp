#include "mainwindow.h"
#include <QApplication>
#include <QDir>

#include "pcbeditor/pcbpalettemanager.h"
#include "pcbeditor/designlayermanager.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("Libre EDA");
    QCoreApplication::setOrganizationDomain("libreeda.org");
    QCoreApplication::setApplicationName("graphicsview-1");

    PcbPaletteManager *paletteManager = PcbPaletteManager::instance();
    paletteManager->setPalettesPath(QDir::currentPath() + "/../../../LibreEDA/src/pcbeditor/settings/");
    paletteManager->loadPalettes();

    DesignLayerManager *layerManager = DesignLayerManager::instance();
    // layerManager->setProfilePath();
    // layerManager->loadProfiles();
    layerManager->loadFromDefaults();

    PcbPalette *palette = paletteManager->activePalette();
    foreach (DesignLayer *layer, layerManager->allLayers()) {
        QColor color = palette->color(PcbPalette::ColorRole(layer->index() + 1));
        layer->setColor(color);
    }

    MainWindow w;
    w.show();

    return a.exec();
}
