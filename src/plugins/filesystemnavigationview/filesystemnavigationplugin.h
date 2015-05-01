#ifndef FILESYSTEMNAVIGATIONPLUGIN_H
#define FILESYSTEMNAVIGATIONPLUGIN_H

#include "core/extension/iplugin.h"

class FileSystemNavigationPlugin : public IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.libre-eda.leda.plugin" FILE "FilesystemNavigationView.json")

public:
    FileSystemNavigationPlugin(QObject *parent = nullptr);
    ~FileSystemNavigationPlugin();

    bool initialize(QString *errorString);
    void extensionsInitialized();
};

#endif // FILESYSTEMNAVIGATIONPLUGIN_H
