#include "graphicsline.h"
#include "designlayer.h"
#include "core/json.h"

#include <QPainter>
#include <QPainterPath>
#include <QPainterPathStroker>

const QString GraphicsLine::J_LINE = QStringLiteral("points");
const QString GraphicsLine::J_WIDTH = QStringLiteral("width");

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
    {
        return;
    }
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
    {
        return;
    }
    prepareGeometryChange();
    m_width = width;
}

bool GraphicsLine::fromJson(QString *errorString, const QJsonObject &jsonObject)
{
    if (!GraphicsItem::fromJson(errorString, jsonObject))
    {
        return false;
    }
    QLineF line;
    if (!Json::toLine(errorString, jsonObject.value(J_LINE), line))
    {
        return false;
    }
    qreal width;
    if (!Json::toReal(errorString, jsonObject.value(J_WIDTH), width))
    {
        return false;
    }
    setLine(line);
    setWidth(width);
    return true;
}

void GraphicsLine::toJson(QJsonObject &jsonObject) const
{
    GraphicsItem::toJson(jsonObject);
    jsonObject.insert(J_LINE, Json::fromLine(m_line));
    jsonObject.insert(J_WIDTH, m_width);
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
    if (!shouldPaint(painter, option, widget))
    {
        return;
    }
    painter->setPen(QPen(QBrush(color(widget)), m_width, Qt::SolidLine, Qt::RoundCap));
    painter->drawLine(m_line);
}
