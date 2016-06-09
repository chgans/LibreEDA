#include "dock/taskdockwidget.h"

#include <QSplitter>
#include <QVBoxLayout>

namespace SymbolEditor
{

    TaskDockWidget::TaskDockWidget(QWidget *parent, Qt::WindowFlags flags) :
        DockWidget(parent, flags)
    {
        setWindowTitle("Task");
        QWidget *widget = new QWidget();
        QVBoxLayout *layout = new QVBoxLayout();
        layout->setMargin(0);
        widget->setLayout(layout);
        setWidget(widget);
    }

    void TaskDockWidget::setTaskWidgets(const QList<QWidget *> widgets)
    {
        for (QWidget *oldWidget : m_widgets)
        {
            widget()->layout()->removeWidget(oldWidget);
            oldWidget->hide();
        }

        m_widgets = widgets;

        for (QWidget *newWidget : m_widgets)
        {
            widget()->layout()->addWidget(newWidget);
            newWidget->show();
        }
    }

    void TaskDockWidget::applySettings(const Settings &settings)
    {
        Q_UNUSED(settings);
    }

}
