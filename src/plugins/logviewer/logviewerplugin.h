#ifndef LOGVIEWERPLUGIN_H
#define LOGVIEWERPLUGIN_H

#include <QObject>

#include "core/extension/iplugin.h"

class LogViewerPlugin : public IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.libre-eda.leda.plugin" FILE "LogViewer.json")

public:
    LogViewerPlugin(QObject *parent = nullptr);
    ~LogViewerPlugin();

    bool initialize(const QStringList &arguments, QString *errorString);
    void extensionsInitialized();
    void shutdown();

private:
};

#endif // LOGVIEWERPLUGIN_H
