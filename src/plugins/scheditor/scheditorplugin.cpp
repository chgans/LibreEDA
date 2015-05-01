#include "scheditorplugin.h"

SchEditorPlugin::SchEditorPlugin(QObject *parent):
    IPlugin(parent)
{

}

SchEditorPlugin::~SchEditorPlugin()
{

}

bool SchEditorPlugin::initialize(QString *errorString)
{
    return true;
}

void SchEditorPlugin::extensionsInitialized()
{

}

