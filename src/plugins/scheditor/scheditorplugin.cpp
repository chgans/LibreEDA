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

    addObject(new SchEditorFactory(this));
    addObject(new SchSettingsPage(this));
    return true;
}

void SchEditorPlugin::extensionsInitialized()
{

}

void SchEditorPlugin::shutdown()
{

}

