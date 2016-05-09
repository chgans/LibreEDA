#include "scheditorplugin.h"
#include "scheditorfactory.h"
#include "schsettingspage.h"

SchEditorPlugin::SchEditorPlugin(QObject *parent):
    IPlugin(parent)
{

}

SchEditorPlugin::~SchEditorPlugin()
{

}

bool SchEditorPlugin::initialize(const QStringList &arguments, QString *errorString)
{
    Q_UNUSED(arguments);
    Q_UNUSED(errorString);

    auto editorFactory = new SchEditorFactory(this);
    auto settingsPage = new SchSettingsPage(this);
    connect(settingsPage, &SchSettingsPage::settingsChanged,
            editorFactory, &SchEditorFactory::loadSettings);

    addObject(editorFactory);
    addObject(settingsPage);

    return true;
}

void SchEditorPlugin::extensionsInitialized()
{

}

void SchEditorPlugin::shutdown()
{

}

