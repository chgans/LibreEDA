#ifndef SCHEDITORPLUGIN_H
#define SCHEDITORPLUGIN_H

#include "core/extension/iplugin.h"

class SchEditorPlugin : public IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.libre-eda.leda.plugin" FILE "SchEditor.json")

public:
    explicit SchEditorPlugin(QObject *parent = nullptr);
    ~SchEditorPlugin();

    bool initialize(const QStringList &arguments, QString *errorString);
    void extensionsInitialized();
    void shutdown();
};

#endif // SCHEDITORPLUGIN_H
