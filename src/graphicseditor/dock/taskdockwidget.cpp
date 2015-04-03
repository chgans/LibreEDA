#include "abstractgraphicsinteractivetool.h"
#include "dock/taskdockwidget.h"
#include "widget/taskeditwidget.h"
#include "widget/taskoptionwidget.h"

#include <QDebug>
#include <QSplitter>
#include <QVBoxLayout>

TaskDockWidget::TaskDockWidget(QWidget *parent) :
    QDockWidget("Task", parent)
{
    QWidget *widget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout();
    widget->setLayout(layout);

    m_splitter = new QSplitter(Qt::Vertical);
    m_taskEditWidget = new TaskEditWidget();
    m_taskOptionWidget = new TaskOptionWidget();
    m_splitter->addWidget(m_taskEditWidget);
    m_splitter->addWidget(m_taskOptionWidget);
    layout->addWidget(m_splitter);

    setWidget(widget);
}

void TaskDockWidget::setTool(AbstractGraphicsInteractiveTool *tool)
{
    setOperationWidget(tool->taskWidget());
    setOptionWidget(tool->optionWidget());
    setFocus();
    focusNextChild();
}

void TaskDockWidget::setOperationWidget(QWidget *widget)
{
    m_taskEditWidget->setCentralWidget(widget);
}

void TaskDockWidget::setOptionWidget(QWidget *widget)
{
    m_taskOptionWidget->setCentralWidget(widget);
}

void TaskDockWidget::focusInEvent(QFocusEvent *event)
{
    qDebug() << __PRETTY_FUNCTION__;
    QDockWidget::focusInEvent(event);
}

void TaskDockWidget::focusOutEvent(QFocusEvent *event)
{
    qDebug() << __PRETTY_FUNCTION__;
    QDockWidget::focusOutEvent(event);
}

bool TaskDockWidget::focusNextPrevChild(bool next)
{
    qDebug() << __PRETTY_FUNCTION__;
    return QDockWidget::focusNextPrevChild(next);
}
