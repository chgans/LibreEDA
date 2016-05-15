#include "tool/interactivetool.h"

#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>

InteractiveTool::InteractiveTool(QObject *parent):
    SchTool(parent)
{

}

InteractiveTool::~InteractiveTool()
{

}

QList<QWidget *> InteractiveTool::optionWidgets()
{
    return m_optionWidgets;
}

void InteractiveTool::setOptionWidgets(QList<QWidget *> widgets)
{
    m_optionWidgets = widgets;
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
