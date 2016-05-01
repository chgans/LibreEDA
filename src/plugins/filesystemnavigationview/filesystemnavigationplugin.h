#ifndef FILESYSTEMNAVIGATIONPLUGIN_H
#define FILESYSTEMNAVIGATIONPLUGIN_H

#include "core/extension/iplugin.h"

class FileSystemNavigationPlugin : public IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.libre-eda.leda.plugin" FILE "FilesystemNavigationView.json")

public:
    explicit FileSystemNavigationPlugin(QObject *parent = nullptr);
    ~FileSystemNavigationPlugin();

    bool initialize(const QStringList &arguments, QString *errorString);
    void extensionsInitialized();
    void shutdown();
};

#endif // FILESYSTEMNAVIGATIONPLUGIN_H
