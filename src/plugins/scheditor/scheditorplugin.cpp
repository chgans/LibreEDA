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
    return true;
}

void SchEditorPlugin::extensionsInitialized()
{

}

void SchEditorPlugin::shutdown()
{

}

