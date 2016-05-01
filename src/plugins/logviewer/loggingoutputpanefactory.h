#ifndef LOGGINGOUTPUTPANEFACTORY_H
#define LOGGINGOUTPUTPANEFACTORY_H

#include "core/outputpane/ioutputpanefactory.h"

class LogModel;
class LoggingOutputPaneFactory : public IOutputPaneFactory
{
    Q_OBJECT
public:
    explicit LoggingOutputPaneFactory(QObject *parent = nullptr);

signals:

public slots:
    void setLogModel(LogModel *model);

private:
    LogModel *m_logModel;

    // IOutputPaneFactory interface
public:
    QWidget *createWidget();
};

#endif // LOGGINGOUTPUTPANEFACTORY_H
