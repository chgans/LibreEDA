#include "graphicscircleitem.h"

#include <QStyleOptionGraphicsItem>

GraphicsCircleItem::GraphicsCircleItem(GraphicsObject *parent):
    GraphicsObject(parent)
{
    //m_centerHandle = addHandle(CenterHandle, CircularHandleShape);
    m_radiusHandle = addHandle(RadiusHandle, DiamondedHandleShape);
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

    markDirty();
    m_radius = length;
    updateHandlesSilently();
}

GraphicsHandle *GraphicsCircleItem::addHandle(GraphicsCircleItem::HandleId handleId, GraphicsHandleShape shape)
{
    GraphicsHandle *handle = new GraphicsHandle(this);
    handle->setRole(MoveHandleRole);
    handle->setHandleShape(shape);
    handle->setPos(QPointF(0, 0));
    addObservedItem(handle);

    m_handleToId[handle] = handleId;
    m_idToHandle[handleId] = handle;

    return handle;
}

void GraphicsCircleItem::updateHandlesSilently()
{
    blockItemNotification();

    m_radiusHandle->setPos(radius(), 0);

    unblockItemNotification();
}

void GraphicsCircleItem::updateGeometry() const
{
    QPainterPath path;
    path.addEllipse(QPointF(0, 0),
                    radius(), radius());
    m_shape = path;

    qreal extra = pen().widthF();
    m_boundingRect.setWidth(2*radius() + extra);
    m_boundingRect.setHeight(2*radius() + extra);
    m_boundingRect.moveCenter(QPointF(0, 0));
    m_dirty = false;
}

void GraphicsCircleItem::markDirty()
{
    prepareGeometryChange();
    m_dirty = true;
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

    Q_ASSERT(handle == m_radiusHandle);
    setRadius(qAbs(handle->pos().x()));
}

QRectF GraphicsCircleItem::boundingRect() const
{
    if (m_dirty)
        updateGeometry();
    return m_boundingRect;
}

QPainterPath GraphicsCircleItem::shape() const
{
    if (m_dirty)
        updateGeometry();
    return m_shape;
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

