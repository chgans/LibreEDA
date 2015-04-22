#include "mainwindow.h"
#include <QApplication>
#include <QDir>

#include "core/editormanager/editormanager.h"
#include "pcbeditor/pcbpalettemanager.h"
#include "pcbeditor/designlayermanager.h"
#include "pcbeditor/pcbeditorfactory.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("LibreEDA");
    QCoreApplication::setOrganizationDomain("libreeda.org");
    QCoreApplication::setApplicationName("leda");

    // Loading plugins
    EditorManager *editorManager = EditorManager::instance();
    PcbEditorFactory *pcbEditorFactory = new PcbEditorFactory();
    editorManager->registerEditorFactory(pcbEditorFactory);

    // Pcb plugin initialisation
    PcbPaletteManager *paletteManager = PcbPaletteManager::instance();
    paletteManager->setPalettesPath(QString(BASE_SETTINGS_DIR"/pcb"));
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

    // Load documents

    // Go!
    MainWindow w;
    w.readSettings();
    w.show();

    return a.exec();
}
