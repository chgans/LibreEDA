#include "mainwindow.h"
#include <QApplication>
#include <QDir>

#include "core/extension/pluginmanager.h"
#include "core/editormanager/editormanager.h"
#include "core/editormanager/documentmanager.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("LibreEDA");
    QCoreApplication::setOrganizationDomain("libreeda.org");
    QCoreApplication::setApplicationName("leda");

    QIcon::setThemeName("breeze");

    // Loading plugins
    PluginManager::setPluginIID("org.libre-eda.leda.plugin");
    PluginManager::setPluginPaths(QStringList() << LEDA_PLUGIN_PATH);
    PluginManager::discoverPlugins();
    PluginManager::loadPlugins();

    // Go!
    MainWindow w;
    w.readSettings();
    w.show();

    return a.exec();
}
