#ifndef TASKDOCKWIDGET_H
#define TASKDOCKWIDGET_H

#include <QDockWidget>

class QSplitter;
class TaskEditWidget;
class TaskOptionWidget;
class AbstractGraphicsInteractiveTool;

class TaskDockWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit TaskDockWidget(QWidget *parent = 0);

signals:

public slots:
    void setTool(AbstractGraphicsInteractiveTool *tool);

private:
    QList<QWidget *> m_widgets;
};

#endif // TASKDOCKWIDGET_H
