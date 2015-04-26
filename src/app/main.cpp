#include "mainwindow.h"
#include <QApplication>
#include <QDir>

#include "core/editormanager/editormanager.h"

// PCB editor
#include "pcbpalettemanager.h"
#include "designlayermanager.h"
#include "pcbeditorfactory.h"

// SCH editor
#include "graphicseditorfactory.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("LibreEDA");
    QCoreApplication::setOrganizationDomain("libreeda.org");
    QCoreApplication::setApplicationName("leda");

    // Loading plugins
    EditorManager *editorManager = EditorManager::instance();
    editorManager->registerEditorFactory(new PcbEditorFactory);
    editorManager->registerEditorFactory(new GraphicsEditorFactory);

    // Pcb plugin initialisation
    PcbPaletteManager *paletteManager = PcbPaletteManager::instance();
    paletteManager->setSystemPath(QString(BASE_SETTINGS_DIR"/pcb/palettes"));
    paletteManager->loadPalettes();
    DesignLayerManager *layerManager = DesignLayerManager::instance();
    layerManager->setSystemPath(QString(BASE_SETTINGS_DIR"/pcb/layersets"));
    layerManager->loadLayerSets();

    // Sch plugin initialisation

    // Load documents

    // Go!
    MainWindow w;
    w.readSettings();
    w.show();

    return a.exec();
}
