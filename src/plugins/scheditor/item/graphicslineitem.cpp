#include "item/graphicslineitem.h"
#include "handle/abstractgraphicshandle.h"

#include <QPainter>
#include <QPen>

PolylineItem::PolylineItem(Item *parent):
    Item(parent)
{
    addRegularHandle(P1Handle, MoveHandleRole, CircularHandleShape);
    addRegularHandle(P2Handle, MoveHandleRole, CircularHandleShape);
}

QLineF PolylineItem::line() const
{
    return m_line;
}

void PolylineItem::setLine(const QLineF &line)
{
    if (m_line == line)
    {
        return;
    }

    prepareGeometryChange();
    m_line = line;
    m_boundingRect = QRectF();
    update();

    blockItemNotification();
    m_idToHandle[P1Handle]->setPos(m_line.p1());
    m_idToHandle[P2Handle]->setPos(m_line.p2());
    unblockItemNotification();

    emit lineChanged();
}

Item *PolylineItem::clone()
{
    PolylineItem *item = new PolylineItem();
    Item::cloneTo(item);
    item->setLine(line());
    return item;
}

QList<QPointF> PolylineItem::endPoints() const
{
    return QList<QPointF>() << m_line.p1() << m_line.p2();
}

QList<QPointF> PolylineItem::midPoints() const
{
    return QList<QPointF>() << m_line.pointAt(0.5);
}

QList<QPointF> PolylineItem::centerPoints() const
{
    return QList<QPointF>() << m_line.pointAt(0.5);
}

QList<QPointF> PolylineItem::nearestPoints(QPointF pos) const
{
    QPointF xPoint;
    QLineF::IntersectType xType = m_line.intersect(m_line.normalVector().translated(pos), &xPoint);
    if (xType == QLineF::BoundedIntersection)
    {
        return QList<QPointF>() << xPoint;
    }
    return QList<QPointF>();
}

void PolylineItem::itemNotification(IObservableItem *item)
{
    Handle *handle = dynamic_cast<Handle *>(item);
    Q_ASSERT(handle);

    QLineF l = m_line;
    if (handle == m_idToHandle[P1Handle])
    {
        l.setP1(handle->pos());
    }
    else
    {
        l.setP2(handle->pos());
    }
    setLine(l);
}

QRectF PolylineItem::boundingRect() const
{
    if (pen().widthF() == 0.0)
    {
        const qreal x1 = m_line.p1().x();
        const qreal x2 = m_line.p2().x();
        const qreal y1 = m_line.p1().y();
        const qreal y2 = m_line.p2().y();
        qreal lx = qMin(x1, x2);
        qreal rx = qMax(x1, x2);
        qreal ty = qMin(y1, y2);
        qreal by = qMax(y1, y2);
        m_boundingRect = QRectF(lx, ty, rx - lx, by - ty);
    }
    else
    {
        m_boundingRect = shape().controlPointRect();
    }

    return m_boundingRect;
}

QPainterPath PolylineItem::shape() const
{
    QPainterPath path;
    if (m_line == QLineF())
    {
        return path;
    }

    path.moveTo(m_line.p1());
    path.lineTo(m_line.p2());

    return shapeFromPath(path, pen());
}

void PolylineItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                             QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(pen());
    painter->drawLine(line());
}

QVariant PolylineItem::itemChange(QGraphicsItem::GraphicsItemChange change,
                                      const QVariant &value)
{
    if (change == QGraphicsItem::ItemSelectedHasChanged)
    {
        for (Handle *handle : m_handleToId.keys())
        {
            handle->setVisible(isSelected());
        }
    }
    return value;
}
