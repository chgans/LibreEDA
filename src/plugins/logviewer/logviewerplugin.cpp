#include "logviewerplugin.h"

LogViewerPlugin::LogViewerPlugin(QObject *parent):
    IPlugin(parent)
{

}

LogViewerPlugin::~LogViewerPlugin()
{
}

bool LogViewerPlugin::initialize(QString *errorString)
{
    return true;
}

void LogViewerPlugin::extensionsInitialized()
{

}

void LogViewerPlugin::shutdown()
{

}

