#include "pcbeditorplugin.h"
#include "pcbeditorfactory.h"
#include "pcbsettingspage.h"

PcbEditorPlugin::PcbEditorPlugin(QObject *parent):
    IPlugin(parent)
{

}

PcbEditorPlugin::~PcbEditorPlugin()
{

}

bool PcbEditorPlugin::initialize(const QStringList &arguments, QString *errorString)
{
    Q_UNUSED(arguments);
    Q_UNUSED(errorString);

    addObject(new PcbEditorFactory(this));
    addObject(new PcbSettingsPage(this));
    return true;
}

void PcbEditorPlugin::extensionsInitialized()
{

}

void PcbEditorPlugin::shutdown()
{

}

