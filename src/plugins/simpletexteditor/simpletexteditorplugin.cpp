#include "simpletexteditorplugin.h"
#include "simpletexteditorfactory.h"

SimpleTextEditorPlugin::SimpleTextEditorPlugin(QObject *parent):
    IPlugin(parent)
{

}

SimpleTextEditorPlugin::~SimpleTextEditorPlugin()
{

}

bool SimpleTextEditorPlugin::initialize(const QStringList &arguments, QString *errorString)
{
    Q_UNUSED(arguments);
    Q_UNUSED(errorString);

    addObject(new SimpleTextEditorFactory(this));
    return true;
}

void SimpleTextEditorPlugin::extensionsInitialized()
{

}

void SimpleTextEditorPlugin::shutdown()
{
}

