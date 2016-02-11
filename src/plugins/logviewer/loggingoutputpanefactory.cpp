#include "loggingoutputpanefactory.h"
#include "logwidget.h"

LoggingOutputPaneFactory::LoggingOutputPaneFactory(QObject *parent):
    IOutputPaneFactory(parent)
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
