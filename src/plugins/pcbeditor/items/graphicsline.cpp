#include "graphicsline.h"
#include "designlayer.h"

#include <QPainter>
#include <QPainterPath>
#include <QPainterPathStroker>

GraphicsLine::GraphicsLine(GraphicsItem *parent):
    GraphicsItem(parent),
    m_width(1000) // 1mm
{

}

GraphicsLine::~GraphicsLine()
{

}

QLineF GraphicsLine::line() const
{
    return m_line;
}

void GraphicsLine::setLine(const QLineF &line)
{
    if (line == m_line)
        return;
    prepareGeometryChange();
    m_line = line;
}

qreal GraphicsLine::width() const
{
    return m_width;
}

void GraphicsLine::setWidth(qreal width)
{
    if (width == m_width)
        return;
    prepareGeometryChange();
    m_width = width;
}

QRectF GraphicsLine::boundingRect() const
{
    return shape().boundingRect();
}

QPainterPath GraphicsLine::shape() const
{
    QPainterPath path;
    path.moveTo(m_line.p1());
    path.lineTo(m_line.p2());
    QPainterPathStroker stroker;
    stroker.setWidth(m_width);
    return stroker.createStroke(path);
}

void GraphicsLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    if (!shouldPaint())
        return;
    painter->setPen(QPen(QBrush(layer()->effectiveColor()), m_width, Qt::SolidLine, Qt::RoundCap));
    painter->drawLine(m_line);
}
