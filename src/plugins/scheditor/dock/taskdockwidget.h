#ifndef TASKDOCKWIDGET_H
#define TASKDOCKWIDGET_H

#include "scheditor/dock/dockwidget.h"

class QSplitter;
class TaskEditWidget;
class TaskOptionWidget;
class InteractiveTool;

class TaskDockWidget : public DockWidget
{
    Q_OBJECT

public:
    explicit TaskDockWidget(QWidget *parent = nullptr);

signals:

public slots:
    void setTool(InteractiveTool *tool);

private:
    QList<QWidget *> m_widgets;

    // DockWidget interface
public slots:
    void applySettings(const SchSettings &settings);
};

#endif // TASKDOCKWIDGET_H
