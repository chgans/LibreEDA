#include "simpletexteditorplugin.h"

SimpleTextEditorPlugin::SimpleTextEditorPlugin(QObject *parent):
    IPlugin(parent)
{

}

SimpleTextEditorPlugin::~SimpleTextEditorPlugin()
{

}

bool SimpleTextEditorPlugin::initialize(const QStringList &arguments, QString *errorString)
{
    return true;
}

void SimpleTextEditorPlugin::extensionsInitialized()
{

}

void SimpleTextEditorPlugin::shutdown()
{

}

