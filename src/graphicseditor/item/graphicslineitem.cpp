#include "graphicslineitem.h"
#include "graphicshandle.h"

#include <QPainter>
#include <QPen>

#include <QDebug>

GraphicsLineItem::GraphicsLineItem(GraphicsObject *parent):
    GraphicsObject(parent), m_dirty(true)
{
    // TODO: same way of doing as bezier item
    // Add handles in mouserelease/move
    m_handle1 = new GraphicsHandle(this);
    m_handle1->setRole(MoveHandleRole);
    m_handle1->setHandleShape(CircularHandleShape);
    m_handle1->setPos(QPointF(0, 0));
    addObservedItem(m_handle1);

    m_handle2 = new GraphicsHandle(this);
    m_handle2->setRole(MoveHandleRole);
    m_handle2->setHandleShape(CircularHandleShape);
    m_handle2->setPos(QPointF(0, 0));
    addObservedItem(m_handle2);
}

QLineF GraphicsLineItem::line() const
{
    return m_line;
}

void GraphicsLineItem::setLine(const QLineF &line)
{
    if (m_line == line)
        return;

    m_line = line;

    blockItemNotification();
    m_handle1->setPos(m_line.p1());
    m_handle2->setPos(m_line.p2());
    unblockItemNotification();

    emit lineChanged();
    markDirty();
}

GraphicsObject *GraphicsLineItem::clone()
{
    GraphicsLineItem *item = new GraphicsLineItem();
    GraphicsObject::cloneTo(item);
    item->setLine(line());
    return item;
}

void GraphicsLineItem::itemNotification(IGraphicsObservableItem *item)
{
    GraphicsHandle *handle = dynamic_cast<GraphicsHandle*>(item);
    Q_ASSERT(handle);

    markDirty();
    QLineF l = m_line;
    if (handle == m_handle1) {
        l.setP1(handle->pos());
    }
    else {
        l.setP2(handle->pos());
    }
    setLine(l);
}

QRectF GraphicsLineItem::boundingRect() const
{
    if (m_dirty)
        updateGeometry();
    return m_boundingRect;
}

QPainterPath GraphicsLineItem::shape() const
{
    if (m_dirty)
        updateGeometry();
    return m_shape;
}

void GraphicsLineItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(pen());
    painter->drawLine(line());
}

QVariant GraphicsLineItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemSelectedHasChanged) {
        foreach (QGraphicsItem *h, childItems()) {
            h->setVisible(isSelected());
        }
    }
    return value;
}

void GraphicsLineItem::markDirty()
{
    prepareGeometryChange();
    m_dirty = true;
}

void GraphicsLineItem::updateGeometry() const
{
    QPainterPath path;
    QPainterPathStroker stroker;
    stroker.setWidth(pen().widthF());
    stroker.setCapStyle(pen().capStyle());
    stroker.setJoinStyle(pen().joinStyle());
    stroker.setMiterLimit(pen().miterLimit());

    path.moveTo(m_line.p1());
    path.lineTo(m_line.p2());

    m_shape = stroker.createStroke(path);

    QRectF rect = m_shape.boundingRect();
    qreal extra = pen().widthF()/2.0;
    m_boundingRect = rect.adjusted(-extra, -extra, +extra, +extra);

    m_dirty = false;
}
