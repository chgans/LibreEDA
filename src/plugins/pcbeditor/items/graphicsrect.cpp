#include "graphicsrect.h"
#include "designlayer.h"
#include "core/json.h"

#include <QPainter>
#include <QJsonObject>

const QString GraphicsRect::J_RECT = QStringLiteral("points");
const QString GraphicsRect::J_CORNER_RADIUS = QStringLiteral("corner-radius");

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

bool GraphicsRect::fromJson(QString *errorString, const QJsonObject &jsonObject)
{
    if (!GraphicsItem::fromJson(errorString, jsonObject))
        return false;
    QRectF rect;
    if (!Json::toRect(errorString, jsonObject.value(J_RECT), rect))
        return false;
    qreal radius = 0;
    if (jsonObject.contains(J_CORNER_RADIUS) && !Json::toReal(errorString, jsonObject.value(J_CORNER_RADIUS), radius))
        return false;
    setRect(rect);
    setCornerRadius(radius);
    return true;
}

void GraphicsRect::toJson(QJsonObject &jsonObject) const
{
    GraphicsItem::toJson(jsonObject);
    jsonObject.insert(J_CORNER_RADIUS, m_cornerRadius);
    jsonObject.insert(J_RECT, Json::fromRect(m_rect));
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
    if (!shouldPaint(painter, option, widget))
        return;
    painter->setPen(Qt::NoPen);
    painter->setBrush(QBrush(color(widget)));
    painter->drawRoundedRect(m_rect, m_cornerRadius, m_cornerRadius);
}
