#include "filesystemnavigationplugin.h"

FileSystemNavigationPlugin::FileSystemNavigationPlugin(QObject *parent):
    IPlugin(parent)
{

}

FileSystemNavigationPlugin::~FileSystemNavigationPlugin()
{

}

bool FileSystemNavigationPlugin::initialize(const QStringList &arguments, QString *errorString)
{
    return true;
}

void FileSystemNavigationPlugin::extensionsInitialized()
{

}

void FileSystemNavigationPlugin::shutdown()
{

}

