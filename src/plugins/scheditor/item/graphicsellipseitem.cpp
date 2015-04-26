#include "graphicsellipseitem.h"

#include <QStyleOptionGraphicsItem>

GraphicsEllipseItem::GraphicsEllipseItem(GraphicsObject *parent):
    GraphicsObject(parent),
    m_xRadius(0.0f), m_yRadius(0.0f)
{
    addRegularHandle(XRadiusHandle, MoveHandleRole, DiamondedHandleShape);
    addRegularHandle(YRadiusHandle, MoveHandleRole, DiamondedHandleShape);
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
    m_boundingRect = QRectF();
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
    m_boundingRect = QRectF();
    update();

    blockItemNotification();
    m_idToHandle[YRadiusHandle]->setPos(0, m_yRadius);
    unblockItemNotification();

    emit yRadiusChanged(yRadius);
}

QRectF GraphicsEllipseItem::boundingRect() const
{
    if (m_boundingRect.isNull()) {
        qreal pw = pen().style() == Qt::NoPen ? qreal(0) : pen().widthF();
        if (pw == 0.0 /*&& m_spanAngle == 360 * 16*/) {
            m_boundingRect.setWidth(2*xRadius());
            m_boundingRect.setHeight(2*yRadius());
        }
        else
            m_boundingRect = shape().controlPointRect();
    }
    return m_boundingRect;
}

QPainterPath GraphicsEllipseItem::shape() const
{
    QPainterPath path;
    path.addEllipse(QPointF(0, 0), m_xRadius, m_yRadius);
    return shapeFromPath(path, pen());
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
        foreach (AbstractGraphicsHandle *handle, m_handleToId.keys()) {
            handle->setVisible(isSelected());
        }
    }
    return value;
}

void GraphicsEllipseItem::itemNotification(IGraphicsObservableItem *item)
{
    AbstractGraphicsHandle *handle = static_cast<AbstractGraphicsHandle *>(item);
    if (handle == m_idToHandle[XRadiusHandle])
        setXRadius(qAbs(handle->pos().x()));
    else
        setYRadius(qAbs(handle->pos().y()));
}
