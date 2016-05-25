#include "item/circleitem.h"

#include <QStyleOptionGraphicsItem>

using namespace SymbolEditor;

CircleItem::CircleItem(Item *parent):
    Item(parent),
    m_radius(0.0)
{
    addRegularHandle(RadiusHandle, MoveHandleRole, DiamondedHandleShape);
}

CircleItem::~CircleItem()
{

}

qreal CircleItem::radius() const
{
    return m_radius;
}

void CircleItem::setRadius(qreal length)
{
    if (qFuzzyCompare(m_radius, length))
    {
        return;
    }

    prepareGeometryChange();
    m_radius = length;
    m_boundingRect = QRectF();
    update();

    blockItemNotification();
    m_idToHandle[RadiusHandle]->setPos(m_radius, 0);
    unblockItemNotification();

    emit radiusChanged();
}

Item *CircleItem::clone()
{
    CircleItem *item = new CircleItem();
    Item::cloneTo(item);
    item->setRadius(radius());
    return item;
}

void CircleItem::itemNotification(IObservableItem *item)
{
    Handle *handle = static_cast<Handle *>(item);

    setRadius(qAbs(handle->pos().x()));
}

QList<QPointF> CircleItem::endPoints() const
{
    return QList<QPointF>();
}

QList<QPointF> CircleItem::midPoints() const
{
    return QList<QPointF>();
}

QList<QPointF> CircleItem::centerPoints() const
{
    return QList<QPointF>() << QPointF(0, 0);
}

QList<QPointF> CircleItem::nearestPoints(QPointF pos) const
{
    QLineF line(QPointF(0, 0), pos);
    line.setLength(m_radius);
    return QList<QPointF>() << line.p2();
}

QRectF CircleItem::boundingRect() const
{
    if (m_boundingRect.isNull())
    {
        qreal pw = pen().style() == Qt::NoPen ? qreal(0) : pen().widthF();
        if (pw == 0.0 /*&& m_spanAngle == 360 * 16*/)
        {
            m_boundingRect.setWidth(2 * m_radius);
            m_boundingRect.setHeight(2 * m_radius);
        }
        else
        {
            m_boundingRect = shape().controlPointRect();
        }
    }

    return m_boundingRect;
}

QPainterPath CircleItem::shape() const
{
    QPainterPath path;
    path.addEllipse(QPointF(0, 0), m_radius, m_radius);
    return shapeFromPath(path, pen());
}

void CircleItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                               QWidget *widget)
{
    Q_UNUSED(widget);

    if (!shape().intersects(option->exposedRect))
    {
        return;
    }

    painter->setClipRect(option->exposedRect);
    painter->setPen(pen());
    painter->setBrush(brush());
    painter->drawEllipse(QPointF(0, 0), radius(), radius());
}

QVariant CircleItem::itemChange(QGraphicsItem::GraphicsItemChange change,
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

