#include "logviewerplugin.h"
#include "loggingoutputpanefactory.h"
#include "logmodel.h"

// FIXME
static LogModel *logModel = nullptr;
static QtMessageHandler previousHandler = nullptr;

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    if (previousHandler != nullptr)
    {
        (*previousHandler)(type, context, msg);
    }

    if (logModel == nullptr)
        return;

    logModel->addMessage(new LogMessage(type, msg, QString(QByteArray(context.category))));
}

LogViewerPlugin::LogViewerPlugin(QObject *parent):
    IPlugin(parent)
{
    if (logModel == nullptr) {
        logModel = new LogModel;
        previousHandler = qInstallMessageHandler(myMessageOutput);
    }
}

LogViewerPlugin::~LogViewerPlugin()
{
    delete logModel;
    logModel = nullptr;
}

bool LogViewerPlugin::initialize(const QStringList &arguments, QString *errorString)
{
    Q_UNUSED(arguments);
    Q_UNUSED(errorString);
    LoggingOutputPaneFactory *factory = new LoggingOutputPaneFactory(this);
    factory->setLogModel(logModel);
    addObject(factory);
    return true;
}

void LogViewerPlugin::extensionsInitialized()
{

}

void LogViewerPlugin::shutdown()
{

}

