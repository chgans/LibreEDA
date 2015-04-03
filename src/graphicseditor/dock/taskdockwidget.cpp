#include "graphicstool.h"
#include "dock/taskdockwidget.h"
#include "widget/taskeditwidget.h"
#include "widget/taskoptionwidget.h"

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

    //layout->addStretch();
    setWidget(widget);

    setFocusPolicy(Qt::StrongFocus);
}

void TaskDockWidget::setTool(GraphicsTool *tool)
{
    setOperationWidget(tool->taskWidget());
    setOptionWidget(tool->optionWidget());
    if (tool->taskWidget())
        tool->taskWidget()->setFocus(Qt::OtherFocusReason);
}

void TaskDockWidget::setOperationWidget(QWidget *widget)
{
    m_taskEditWidget->setCentralWidget(widget);
}

void TaskDockWidget::setOptionWidget(QWidget *widget)
{
    m_taskOptionWidget->setCentralWidget(widget);
}
