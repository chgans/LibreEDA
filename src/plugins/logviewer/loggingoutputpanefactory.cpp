#include "loggingoutputpanefactory.h"
#include "logwidget.h"

LoggingOutputPaneFactory::LoggingOutputPaneFactory(QObject *parent):
    IOutputPaneFactory(parent), m_logModel(nullptr)
{
    setPriority(8);
    setDisplayName("General Messages");
}

void LoggingOutputPaneFactory::setLogModel(LogModel *model)
{
    m_logModel = model;
}

QWidget *LoggingOutputPaneFactory::createWidget()
{
    LogWidget *widget = new LogWidget;
    widget->setModel(m_logModel);
    return widget;
}
