#include "graphicswireitem.h"
#include "graphicshandle.h"
#include "graphicsscene.h"

#include <QGraphicsPathItem>

#include <QDebug>

GraphicsWireItem::GraphicsWireItem(GraphicsObject *parent):
    GraphicsObject(parent), IGraphicsItemObserver()
{
}

GraphicsWireItem::~GraphicsWireItem()
{

}

QRectF GraphicsWireItem::boundingRect() const
{
    if (m_boundingRect.isNull()) {
        qreal pw = pen().style() == Qt::NoPen ? qreal(0) : pen().widthF();
        if (pw == 0.0)
            m_boundingRect = m_path.boundingRect();
        else
            m_boundingRect = shape().controlPointRect();
    }
    return m_boundingRect;
}

QPainterPath GraphicsWireItem::shape() const
{
    QPainterPath path = shapeFromPath(m_path, pen());
    return path;
}

void GraphicsWireItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(pen());
    painter->setBrush(QBrush());
    painter->drawPath(m_path);
}

GraphicsObject *GraphicsWireItem::clone()
{
    GraphicsWireItem *item = new GraphicsWireItem();
    item->setPoints(points());
    cloneTo(item);
    return item;
}

QList<QPointF> GraphicsWireItem::points() const
{
    QList<QPointF> result;
    foreach (GraphicsHandle *handle, m_handles)
        result.append(handle->pos());
    return result;
}


void GraphicsWireItem::addPoint(const QPointF &pos)
{
    addHandle(pos);
    handleToPath();

    emit pointsChanged();
}

void GraphicsWireItem::movePoint(int idx, const QPointF &pos)
{
    Q_ASSERT(idx < m_handles.count());

    blockItemNotification();
    m_handles[idx]->setPos(pos);
    unblockItemNotification();
    handleToPath();

    emit pointsChanged();
}

void GraphicsWireItem::setPoints(QList<QPointF> points)
{
    blockItemNotification();
    foreach (GraphicsHandle *handle, m_handles) {
        removeObservedItem(handle);
        handle->setParentItem(nullptr);
        delete handle;
    }
    unblockItemNotification();

    m_handles.clear();
    foreach (const QPointF &pos, points)
        addHandle(pos);
    handleToPath();

    emit pointsChanged();
}

void GraphicsWireItem::handleToPath()
{
    prepareGeometryChange();
    m_path = QPainterPath();
    if (!m_handles.isEmpty()) {
        m_path.moveTo(m_handles[0]->pos());
        for (int i = 1; i < m_handles.count(); i++) {
            m_path.lineTo(m_handles[i]->pos());
        }
    }
    m_boundingRect = QRectF();
    update();
}

void GraphicsWireItem::pathToHandle()
{
    blockItemNotification();
    for (int i = 0; i < m_path.elementCount(); i++) {
        m_handles[i]->setPos(m_path.elementAt(i).x,
                             m_path.elementAt(i).y);
    }
    unblockItemNotification();
}

GraphicsHandle *GraphicsWireItem::addHandle(const QPointF &pos)
{
    GraphicsHandle *handle = new GraphicsHandle(this);
    handle->setHandleShape(CircularHandleShape);
    handle->setRole(MoveHandleRole);
    handle->setPos(pos);
    m_handles.append(handle);
    blockItemNotification();
    addObservedItem(handle);
    unblockItemNotification();
    return handle;
}


void GraphicsWireItem::itemNotification(IGraphicsObservableItem *item)
{
    Q_UNUSED(item);
    handleToPath();
}


QVariant GraphicsWireItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemSelectedHasChanged) {
        foreach (GraphicsHandle *handle, m_handles) {
            handle->setVisible(isSelected());
        }
    }
    return value;
}
