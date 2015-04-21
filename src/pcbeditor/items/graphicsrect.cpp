#include "graphicsrect.h"
#include "designlayer.h"

#include <QPainter>

GraphicsRect::GraphicsRect(GraphicsItem *parent):
    GraphicsItem(parent),
    m_cornerRadius(0.0)
{

}

GraphicsRect::~GraphicsRect()
{

}

QRectF GraphicsRect::rect() const
{
    return m_rect;
}

void GraphicsRect::setRect(const QRectF &rect)
{
    if (m_rect == rect)
        return;
    m_rect = rect;
    prepareGeometryChange();
}

qreal GraphicsRect::cornerRadius() const
{
    return m_cornerRadius;
}

void GraphicsRect::setCornerRadius(qreal radius)
{
    if (qFuzzyCompare(radius + 1, m_cornerRadius + 1))
        return;
    m_cornerRadius = radius;
    prepareGeometryChange();
}

QRectF GraphicsRect::boundingRect() const
{
    return m_rect;
}

QPainterPath GraphicsRect::shape() const
{
    QPainterPath path;
    path.addRoundedRect(m_rect, m_cornerRadius, m_cornerRadius);
    return path;
}

void GraphicsRect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    if (!shouldPaint())
        return;
    painter->setPen(Qt::NoPen);
    painter->setBrush(QBrush(layer()->effectiveColor()));
    painter->drawRoundedRect(m_rect, m_cornerRadius, m_cornerRadius);
}
