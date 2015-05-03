#ifndef SCHEDITORPLUGIN_H
#define SCHEDITORPLUGIN_H

#include "core/extension/iplugin.h"

class SchEditorPlugin : public IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.libre-eda.leda.plugin" FILE "SchEditor.json")

public:
    SchEditorPlugin(QObject *parent = nullptr);
    ~SchEditorPlugin();

    bool initialize(QString *errorString);
    void extensionsInitialized();
    void shutdown();
};

#endif // SCHEDITORPLUGIN_H
