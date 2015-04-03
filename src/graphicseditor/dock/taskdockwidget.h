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

protected:
    void setOperationWidget(QWidget *widget);
    void setOptionWidget(QWidget *widget);

private:
    QSplitter *m_splitter;
    TaskEditWidget *m_taskEditWidget;
    TaskOptionWidget *m_taskOptionWidget;

    // QWidget interface
protected:
    void focusInEvent(QFocusEvent *event);
    void focusOutEvent(QFocusEvent *event);
    bool focusNextPrevChild(bool next);
};

#endif // TASKDOCKWIDGET_H
