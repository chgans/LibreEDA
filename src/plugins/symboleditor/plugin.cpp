#include "plugin.h"
#include "editorfactory.h"
#include "settings/appearancesettingspage.h"

// See Q_INIT_RESOURCE documentation for why this is done this way
inline void SymbolEditor_InitialiseResources()
{
    Q_INIT_RESOURCE(symboleditor);
}

namespace SymbolEditor
{

    Plugin::Plugin(QObject *parent):
        IPlugin(parent)
    {
        SymbolEditor_InitialiseResources();
    }

    Plugin::~Plugin()
    {

    }

    bool Plugin::initialize(const QStringList &arguments, QString *errorString)
    {
        Q_UNUSED(arguments);
        Q_UNUSED(errorString);

        m_editorFactory = new EditorFactory(this);
        m_settingsPage = new AppearanceSettingsPage(this);
        connect(m_settingsPage, &AppearanceSettingsPage::settingsChanged,
                this, &Plugin::applySettings);

        addObject(m_editorFactory);
        addObject(m_settingsPage);

        return true;
    }

    void Plugin::extensionsInitialized()
    {

    }

    void Plugin::shutdown()
    {

    }

    void Plugin::applySettings()
    {
        m_editorFactory->applySettings();
    }

}
