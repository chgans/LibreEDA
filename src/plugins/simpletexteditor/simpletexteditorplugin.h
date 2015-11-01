#ifndef SIMPLETEXTEDITORPLUGIN_H
#define SIMPLETEXTEDITORPLUGIN_H

#include "core/extension/iplugin.h"

class SimpleTextEditorPlugin : public IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.libre-eda.leda.plugin" FILE "SimpleTextEditor.json")

public:
    SimpleTextEditorPlugin(QObject *parent = nullptr);
    ~SimpleTextEditorPlugin();

    bool initialize(const QStringList &arguments, QString *errorString);
    void extensionsInitialized();
    void shutdown();
};

#endif // SIMPLETEXTEDITORPLUGIN_H
