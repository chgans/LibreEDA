#include "filesystemnavigationplugin.h"
#include "filesystemnavigationviewfactory.h"

FileSystemNavigationPlugin::FileSystemNavigationPlugin(QObject *parent):
    IPlugin(parent)
{

}

FileSystemNavigationPlugin::~FileSystemNavigationPlugin()
{

}

bool FileSystemNavigationPlugin::initialize(const QStringList &arguments, QString *errorString)
{
    addObject(new FileSystemNavigationViewFactory(this));
    return true;
}

void FileSystemNavigationPlugin::extensionsInitialized()
{

}

void FileSystemNavigationPlugin::shutdown()
{

}

