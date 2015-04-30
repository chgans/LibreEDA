#include "mainwindow.h"
#include <QApplication>
#include <QDir>

#include "core/editormanager/editormanager.h"
#include "core/editormanager/documentmanager.h"

// PCB editor
#include "pcbeditor/pcbpalettemanager.h"
#include "pcbeditor/designlayermanager.h"
#include "pcbeditor/pcbeditorfactory.h"

// SCH editor
#include "scheditor/scheditorfactory.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("LibreEDA");
    QCoreApplication::setOrganizationDomain("libreeda.org");
    QCoreApplication::setApplicationName("leda");

    // Loading plugins
    EditorManager::registerEditorFactory(new PcbEditorFactory);
    EditorManager::registerEditorFactory(new SchEditorFactory);
    DocumentManager::loadSettings();

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
