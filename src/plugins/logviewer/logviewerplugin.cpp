#include "logviewerplugin.h"

LogViewerPlugin::LogViewerPlugin(QObject *parent):
    IPlugin(parent)
{

}

LogViewerPlugin::~LogViewerPlugin()
{
}

bool LogViewerPlugin::initialize(const QStringList &arguments, QString *errorString)
{
    return true;
}

void LogViewerPlugin::extensionsInitialized()
{

}

void LogViewerPlugin::shutdown()
{

}

