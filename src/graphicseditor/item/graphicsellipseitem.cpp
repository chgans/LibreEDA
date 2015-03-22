#include "graphicsellipseitem.h"

#include <QStyleOptionGraphicsItem>

GraphicsEllipseItem::GraphicsEllipseItem(GraphicsObject *parent):
    GraphicsObject(parent),
    m_xRadius(0.0f), m_yRadius(0.0f)
{

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

    m_xRadius = xRadius;
    emit xRadiusChanged(xRadius);
}

void GraphicsEllipseItem::setYRadius(qreal yRadius)
{
    if (qFuzzyCompare(m_yRadius, yRadius))
        return;

    m_yRadius = yRadius;
    emit yRadiusChanged(yRadius);
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
