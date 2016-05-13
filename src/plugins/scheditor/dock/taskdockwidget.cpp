#include "tool/interactivetool.h"
#include "dock/taskdockwidget.h"
#include "widget/taskeditwidget.h"
#include "widget/taskoptionwidget.h"

#include <QSplitter>
#include <QVBoxLayout>

TaskDockWidget::TaskDockWidget(QWidget *parent, Qt::WindowFlags flags) :
    DockWidget(parent, flags)
{
    setWindowTitle("Task");
    QWidget *widget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout();
    widget->setLayout(layout);
    setWidget(widget);
}

void TaskDockWidget::setTool(InteractiveTool *tool)
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

void TaskDockWidget::applySettings(const SchEditorSettings &settings)
{
    Q_UNUSED(settings);
}
