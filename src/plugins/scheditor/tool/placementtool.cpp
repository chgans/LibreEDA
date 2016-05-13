#include "tool/placementtool.h"

#include "item/schitem.h"
#include "schscene.h"
#include "schview.h"

#include <QMouseEvent>
#include <QKeyEvent>
#include <QWheelEvent>

#include <QAction>

#include <QDialog>

PlacementTool::PlacementTool(QObject *parent):
    InteractiveTool(parent)
{
    resetTool();
}

PlacementTool::~PlacementTool()
{

}

// TODO: tools work in scene coordinates, the view have to do the mapping
QPointF PlacementTool::mapToScene(const QPoint &pos)
{
    return view()->mapToScene(pos);
}

QPointF PlacementTool::mapFromScene(const QPointF &pos)
{
    return view()->mapFromScene(pos);
}


QPointF PlacementTool::mapToItem(const QPoint &pos)
{
    if (m_item == nullptr)
    {
        return QPointF();
    }
    return m_item->mapFromScene(mapToScene(pos));
}

QPointF PlacementTool::mapFromItem(const QPointF &pos)
{
    if (m_item == nullptr)
    {
        return QPointF();
    }
    return m_item->mapToScene(mapFromScene(pos));
}

void PlacementTool::resetTool()
{
    m_item = nullptr;
    m_addPointOnMouseMove = false;
    m_isActive = false;
    m_index = -1;
}

void PlacementTool::goBack()
{
    if (m_index >= 0)
    {
        if (removePoint(m_index, m_movePos))
        {
            m_index--;
            // If hit here, then it means that the tool's removePoint forgot to return false
            Q_ASSERT(m_index >= 0);
        }
        else
        {
            scene()->removeItem(m_item);
            resetTool();
        }
    }
    else
    {
        emit finished();
    }
}

void PlacementTool::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
    {
        return;
    }

    if (!m_isActive)
    {
        m_isActive = true;
    }

    m_pressPos = event->pos();

    if (m_index < 0)
    {
        scene()->clearSelection();
    }

    event->accept();
}

void PlacementTool::mouseMoveEvent(QMouseEvent *event)
{
    if (!m_isActive || m_index < 0)
    {
        return;
    }

    m_movePos = event->pos();

    if (!m_addPointOnMouseMove)
    {
        Q_ASSERT(m_index != 0);
        movePoint(m_index, m_movePos);
        return;
    }

    m_addPointOnMouseMove = false;

    freezePoint(m_index, m_movePos);
    if (!m_isActive)
    {
        return;
    }

    m_index ++;
    addPoint(m_index, m_movePos);
}

void PlacementTool::mouseReleaseEvent(QMouseEvent *event)
{
    if (!m_isActive)
    {
        return;
    }

    if (event->button() != Qt::LeftButton)
    {
        return;
    }

    if (m_index < 0)
    {
        scene()->clearSelection();
        m_item = beginInsert(m_pressPos);
        Q_ASSERT(m_item != nullptr);
        scene()->addItem(m_item);
        m_index = 0;
        addPoint(m_index, m_pressPos);
        freezePoint(m_index, m_pressPos);
    }
    m_addPointOnMouseMove = true;
}

void PlacementTool::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event);

    if (!m_isActive)
    {
        return;
    }

    endInsert(m_pressPos);
}

void PlacementTool::activate(const QAction *which, SchView *view)
{
    Q_UNUSED(which);
    Q_UNUSED(view);
}

void PlacementTool::desactivate(const QAction *which, SchView *view)
{
    Q_UNUSED(which);
    Q_UNUSED(view);
}


void PlacementTool::keyPressEvent(QKeyEvent *event)
{
    Q_UNUSED(event);
}

void PlacementTool::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        goBack();
        event->accept();
    }
}
