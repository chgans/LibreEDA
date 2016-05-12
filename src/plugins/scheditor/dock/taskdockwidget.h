#ifndef TASKDOCKWIDGET_H
#define TASKDOCKWIDGET_H

#include <QDockWidget>

class QSplitter;
class TaskEditWidget;
class TaskOptionWidget;
class InteractiveTool;

class TaskDockWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit TaskDockWidget(QWidget *parent = nullptr);

signals:

public slots:
    void setTool(InteractiveTool *tool);

private:
    QList<QWidget *> m_widgets;
};

#endif // TASKDOCKWIDGET_H
