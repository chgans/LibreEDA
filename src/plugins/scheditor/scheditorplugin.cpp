#include "scheditorplugin.h"
#include "scheditorfactory.h"
#include "schsettingspage.h"

// See Q_INIT_RESOURCE documentation for why this is done this way
inline void SymbolEditor_InitialiseResources()
{
    Q_INIT_RESOURCE(scheditor);
}

using namespace SymbolEditor;

SchEditorPlugin::SchEditorPlugin(QObject *parent):
    IPlugin(parent)
{
    SymbolEditor_InitialiseResources();
}

SchEditorPlugin::~SchEditorPlugin()
{

}

bool SchEditorPlugin::initialize(const QStringList &arguments, QString *errorString)
{
    Q_UNUSED(arguments);
    Q_UNUSED(errorString);

    m_editorFactory = new SchEditorFactory(this);
    m_settingsPage = new SchSettingsPage(this);
    connect(m_settingsPage, &SchSettingsPage::settingsChanged,
            this, &SchEditorPlugin::applySettings);

    addObject(m_editorFactory);
    addObject(m_settingsPage);

    return true;
}

void SchEditorPlugin::extensionsInitialized()
{

}

void SchEditorPlugin::shutdown()
{

}

void SchEditorPlugin::applySettings()
{
    m_editorFactory->applySettings();
}

