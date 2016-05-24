#include "tool/interactivetool.h"

#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>

InteractiveTool::InteractiveTool(QObject *parent):
    Tool(parent)
{

}

InteractiveTool::~InteractiveTool()
{

}

QList<QWidget *> InteractiveTool::taskWidgets()
{
    return m_taskWidgets;
}

void InteractiveTool::setTaskWidgets(QList<QWidget *> widgets)
{
    m_taskWidgets = widgets;
    emit taskWidgetsChanged(m_taskWidgets);
}

void InteractiveTool::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void InteractiveTool::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void InteractiveTool::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void InteractiveTool::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void InteractiveTool::keyPressEvent(QKeyEvent *event)
{
    Q_UNUSED(event);
}

void InteractiveTool::keyReleaseEvent(QKeyEvent *event)
{
    Q_UNUSED(event);
}

void InteractiveTool::wheelEvent(QWheelEvent *event)
{
    Q_UNUSED(event);
}
