#pragma once

#include "core/extension/iplugin.h"

namespace SymbolEditor
{

    class EditorFactory;
    class AppearanceSettingsPage;

    class Plugin : public IPlugin
    {
        Q_OBJECT
        Q_PLUGIN_METADATA(IID "org.libre-eda.leda.plugin" FILE "SymbolEditor.json")

    public:
        explicit Plugin(QObject *parent = nullptr);
        ~Plugin();

        bool initialize(const QStringList &arguments, QString *errorString);
        void extensionsInitialized();
        void shutdown();

    public slots:
        void applySettings();

    private:
        EditorFactory *m_editorFactory;
        AppearanceSettingsPage *m_settingsPage;
    };

}
