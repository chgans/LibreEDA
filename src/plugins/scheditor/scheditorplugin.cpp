#include "scheditorplugin.h"
#include "scheditorfactory.h"
#include "schsettingspage.h"

SchEditorPlugin::SchEditorPlugin(QObject *parent):
    IPlugin(parent)
{
    Q_INIT_RESOURCE(scheditor);
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

