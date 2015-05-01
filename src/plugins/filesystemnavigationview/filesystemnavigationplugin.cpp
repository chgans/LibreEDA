#include "filesystemnavigationplugin.h"

FileSystemNavigationPlugin::FileSystemNavigationPlugin(QObject *parent):
    IPlugin(parent)
{

}

FileSystemNavigationPlugin::~FileSystemNavigationPlugin()
{

}

bool FileSystemNavigationPlugin::initialize(QString *errorString)
{
    return true;
}

void FileSystemNavigationPlugin::extensionsInitialized()
{

}

