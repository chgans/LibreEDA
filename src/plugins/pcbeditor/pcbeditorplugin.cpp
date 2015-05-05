#include "pcbeditorplugin.h"

PcbEditorPlugin::PcbEditorPlugin(QObject *parent):
    IPlugin(parent)
{

}

PcbEditorPlugin::~PcbEditorPlugin()
{

}

bool PcbEditorPlugin::initialize(const QStringList &arguments, QString *errorString)
{
    return true;
}

void PcbEditorPlugin::extensionsInitialized()
{

}

void PcbEditorPlugin::shutdown()
{

}

