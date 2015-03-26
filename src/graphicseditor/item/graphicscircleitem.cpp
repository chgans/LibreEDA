#include "graphicscircleitem.h"

#include <QStyleOptionGraphicsItem>

GraphicsCircleItem::GraphicsCircleItem(GraphicsObject *parent):
    GraphicsObject(parent)
{
    addHandle(RadiusHandle, MoveHandleRole, DiamondedHandleShape);
}

GraphicsCircleItem::~GraphicsCircleItem()
{

}

qreal GraphicsCircleItem::radius() const
{
    return m_radius;
}

void GraphicsCircleItem::setRadius(qreal length)
{
    if (qFuzzyCompare(m_radius, length))
        return;

    prepareGeometryChange();
    m_radius = length;
    m_boundingRect = QRectF();
    update();

    blockItemNotification();
    m_idToHandle[RadiusHandle]->setPos(m_radius, 0);
    unblockItemNotification();

    emit radiusChanged();
}

GraphicsObject *GraphicsCircleItem::clone()
{
    GraphicsCircleItem *item = new GraphicsCircleItem();
    GraphicsObject::cloneTo(item);
    item->setRadius(radius());
    return item;
}

void GraphicsCircleItem::itemNotification(IGraphicsObservableItem *item)
{
    GraphicsHandle *handle = static_cast<GraphicsHandle *>(item);

    setRadius(qAbs(handle->pos().x()));
}

QRectF GraphicsCircleItem::boundingRect() const
{
    if (m_boundingRect.isNull()) {
        qreal pw = pen().style() == Qt::NoPen ? qreal(0) : pen().widthF();
        if (pw == 0.0 /*&& m_spanAngle == 360 * 16*/) {
            m_boundingRect.setWidth(2 * m_radius);
            m_boundingRect.setHeight(2 * m_radius);
        }
        else
            m_boundingRect = shape().controlPointRect();
    }

    return m_boundingRect;
}

QPainterPath GraphicsCircleItem::shape() const
{
    QPainterPath path;
    path.addEllipse(QPointF(0, 0), m_radius, m_radius);
    return shapeFromPath(path, pen());
}

void GraphicsCircleItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    if (!shape().intersects(option->exposedRect))
        return;

    painter->setClipRect(option->exposedRect);
    painter->setPen(pen());
    painter->setBrush(brush());
    painter->drawEllipse(QPointF(0, 0), radius(), radius());
}

QVariant GraphicsCircleItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemSelectedHasChanged) {
        foreach (QGraphicsItem *h, childItems()) {
            h->setVisible(isSelected());
        }
    }
    return value;
}

