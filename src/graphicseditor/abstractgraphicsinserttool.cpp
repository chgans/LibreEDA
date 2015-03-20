#include "abstractgraphicsinserttool.h"

#include "graphicsobject.h"
#include "graphicsscene.h"
#include "graphicsview.h"

#include <QMouseEvent>
#include <QKeyEvent>
#include <QWheelEvent>

#include <QDebug>

AbstractGraphicsInsertTool::AbstractGraphicsInsertTool(QObject *parent):
    AbstractGraphicsInteractiveTool(parent)
{
    resetTool();
}

AbstractGraphicsInsertTool::~AbstractGraphicsInsertTool()
{

}

// TODO: tools work in scene coordinates, the view have to do the mapping
QPointF AbstractGraphicsInsertTool::mapToScene(const QPoint &pos)
{
    return view()->mapToScene(pos);
}

QPointF AbstractGraphicsInsertTool::mapFromScene(const QPointF &pos)
{
    return view()->mapFromScene(pos);
}


QPointF AbstractGraphicsInsertTool::mapToItem(const QPoint &pos)
{
    if (m_item == nullptr)
        return QPointF();
    return m_item->mapFromScene(mapToScene(pos));
}

QPointF AbstractGraphicsInsertTool::mapFromItem(const QPointF &pos)
{
    if (m_item == nullptr)
        return QPointF();
    return m_item->mapToScene(mapFromScene(pos));
}

void AbstractGraphicsInsertTool::resetTool()
{
    m_item = nullptr;
    m_addPointOnMouseMove = false;
    m_isActive = false;
    m_index = -1;
}

void AbstractGraphicsInsertTool::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton) {
        return;
    }

    if (!m_isActive) {
        m_isActive = true;
    }

    m_pressPos = mapToScene(event->pos());

    if (m_index < 0)
        scene()->clearSelection();

    event->accept();
}

void AbstractGraphicsInsertTool::mouseMoveEvent(QMouseEvent *event)
{
    if (!m_isActive || m_index < 0)
        return;

    m_movePos = mapToScene(event->pos());

    if (!m_addPointOnMouseMove) {
        Q_ASSERT(m_index != 0);
        movePoint(m_index, m_movePos);
        return;
    }

    m_addPointOnMouseMove = false;

    freezePoint(m_index);
    if (!m_isActive)
        return;

    m_index ++;
    addPoint(m_index, m_movePos);
}

void AbstractGraphicsInsertTool::mouseReleaseEvent(QMouseEvent *event)
{
    if (!m_isActive)
        return;

    if (event->button() != Qt::LeftButton) {
        return;
    }

    if (m_index < 0) {
        scene()->clearSelection();
        m_item = beginInsert(m_pressPos);
        Q_ASSERT(m_item != nullptr);
        scene()->addItem(m_item);
        m_index = 0;
        addPoint(m_index, m_pressPos);
        freezePoint(m_index);
    }
    m_addPointOnMouseMove = true;
}

void AbstractGraphicsInsertTool::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (!m_isActive)
        return;

    endInsert(m_pressPos);
}

