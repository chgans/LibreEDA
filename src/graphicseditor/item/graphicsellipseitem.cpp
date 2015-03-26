#include "graphicsellipseitem.h"

#include <QStyleOptionGraphicsItem>

GraphicsEllipseItem::GraphicsEllipseItem(GraphicsObject *parent):
    GraphicsObject(parent), IGraphicsItemObserver(),
    m_xRadius(0.0f), m_yRadius(0.0f)
{
    addHandle(XRadiusHandle);
    addHandle(YRadiusHandle);
}

GraphicsEllipseItem::~GraphicsEllipseItem()
{

}

qreal GraphicsEllipseItem::xRadius() const
{
    return m_xRadius;
}

qreal GraphicsEllipseItem::yRadius() const
{
    return m_yRadius;
}

void GraphicsEllipseItem::setXRadius(qreal xRadius)
{
    if (qFuzzyCompare(m_xRadius, xRadius))
        return;

    prepareGeometryChange();
    m_xRadius = xRadius;
    update();

    blockItemNotification();
    m_idToHandle[XRadiusHandle]->setPos(m_xRadius, 0);
    unblockItemNotification();

    emit xRadiusChanged(xRadius);
}

void GraphicsEllipseItem::setYRadius(qreal yRadius)
{
    if (qFuzzyCompare(m_yRadius, yRadius))
        return;

    prepareGeometryChange();
    m_yRadius = yRadius;
    update();

    blockItemNotification();
    m_idToHandle[YRadiusHandle]->setPos(0, m_yRadius);
    unblockItemNotification();

    emit yRadiusChanged(yRadius);
}

GraphicsHandle *GraphicsEllipseItem::addHandle(GraphicsEllipseItem::HandleId handleId)
{
    GraphicsHandle *handle = new GraphicsHandle(this);
    handle->setHandleShape(DiamondedHandleShape);
    handle->setRole(MoveHandleRole);
    m_handleToId[handle] = handleId;
    m_idToHandle[handleId] = handle;
    blockItemNotification();
    addObservedItem(handle);
    unblockItemNotification();
    return handle;
}

QRectF GraphicsEllipseItem::boundingRect() const
{
    qreal extra = pen().widthF();
    QRectF rect;
    rect.setWidth(2*xRadius() + extra);
    rect.setHeight(2*yRadius() + extra);
    rect.moveCenter(QPointF(0, 0));

    return rect;
}

QPainterPath GraphicsEllipseItem::shape() const
{
    QPainterPath path;
    path.addEllipse(boundingRect());
    return path;
}

void GraphicsEllipseItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    painter->setPen(pen());
    painter->setBrush(brush());
    painter->setClipRect(option->exposedRect);
    painter->drawEllipse(QPointF(0, 0), xRadius(), yRadius());
}

GraphicsObject *GraphicsEllipseItem::clone()
{
    GraphicsEllipseItem *item = new GraphicsEllipseItem();
    item->setXRadius(xRadius());
    item->setYRadius(yRadius());
    GraphicsObject::cloneTo(item);
    return item;
}

QVariant GraphicsEllipseItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemSelectedHasChanged) {
        foreach (GraphicsHandle *handle, m_handleToId.keys()) {
            handle->setVisible(isSelected());
        }
    }
    return value;
}

void GraphicsEllipseItem::itemNotification(IGraphicsObservableItem *item)
{
    Q_UNUSED(item);
    setXRadius(qAbs(m_idToHandle[XRadiusHandle]->pos().x()));
    setYRadius(qAbs(m_idToHandle[YRadiusHandle]->pos().y()));
}
