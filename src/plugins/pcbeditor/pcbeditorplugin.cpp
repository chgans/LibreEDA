#include "pcbeditorplugin.h"

PcbEditorPlugin::PcbEditorPlugin(QObject *parent):
    IPlugin(parent)
{

}

PcbEditorPlugin::~PcbEditorPlugin()
{

}

bool PcbEditorPlugin::initialize(QString *errorString)
{
    return true;
}

void PcbEditorPlugin::extensionsInitialized()
{

}

