#ifndef SCHEDITORPLUGIN_H
#define SCHEDITORPLUGIN_H

#include "core/extension/iplugin.h"

namespace SymbolEditor
{

    class SchEditorFactory;
    class SchSettingsPage;
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

    public slots:
        void applySettings();

    private:
        SchEditorFactory *m_editorFactory;
        SchSettingsPage *m_settingsPage;
    };

}

#endif // SCHEDITORPLUGIN_H
