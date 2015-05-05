#ifndef PCBEDITORPLUGIN_H
#define PCBEDITORPLUGIN_H

#include "core/extension/iplugin.h"

class PcbEditorPlugin : public IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.libre-eda.leda.plugin" FILE "PcbEditor.json")

public:
    PcbEditorPlugin(QObject *parent = nullptr);
    ~PcbEditorPlugin();

    bool initialize(const QStringList &arguments, QString *errorString);
    void extensionsInitialized();
    void shutdown();
};

#endif // PCBEDITORPLUGIN_H
