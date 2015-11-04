#include "scheditorplugin.h"

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

    addObject(new SchEditorPlugin(this));
    return true;
}

void SchEditorPlugin::extensionsInitialized()
{

}

void SchEditorPlugin::shutdown()
{

}

