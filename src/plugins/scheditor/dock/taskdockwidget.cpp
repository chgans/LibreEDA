#include "tool/abstractgraphicsinteractivetool.h"
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
    setWidget(widget);
}

void TaskDockWidget::setTool(AbstractGraphicsInteractiveTool *tool)
{
    foreach (QWidget *oldWidget, m_widgets) {
        widget()->layout()->removeWidget(oldWidget);
        oldWidget->hide();
    }
    m_widgets = tool->optionWidgets();
    foreach (QWidget *newWidget, m_widgets) {
        widget()->layout()->addWidget(newWidget);
        newWidget->show();
    }
}
