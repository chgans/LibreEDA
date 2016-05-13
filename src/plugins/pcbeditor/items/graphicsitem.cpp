#include "graphicsitem.h"
#include "views/layoutview.h"
#include "core/json.h"

#include <QStyleOptionGraphicsItem>

const QString GraphicsItem::J_POSITION = QStringLiteral("position");
const QString GraphicsItem::J_ROTATION = QStringLiteral("rotation");
const QString GraphicsItem::J_LAYER = QStringLiteral("layer");
const QString GraphicsItem::J_LOCKED = QStringLiteral("locked");

GraphicsItem::GraphicsItem(GraphicsItem *parent):
    QGraphicsItem(parent),
    m_layer(0),
    m_isLocked(false)
{
}

GraphicsItem::~GraphicsItem()
{

}

int GraphicsItem::layer() const
{
    return m_layer;
}

void GraphicsItem::setLayer(int layer)
{
    if (m_layer == layer)
    {
        return;
    }
    prepareGeometryChange();
    m_layer = layer;
}

// FIXME: item can be non-movable and/or non-selectable independently of being locked
bool GraphicsItem::isLocked() const
{
    return m_isLocked;
}

void GraphicsItem::setLocked(bool locked)
{
    if (locked == m_isLocked)
    {
        return;
    }
    m_isLocked = locked;
}

LayoutView *GraphicsItem::paintedView(QWidget *paintedWidget) const
{
    // TODO: static_cast once all views inherit from AbstractLayoutView
    return qobject_cast<LayoutView *>(paintedWidget);
}

bool GraphicsItem::shouldPaint(const QPainter *painter, const QStyleOptionGraphicsItem *option,
                               QWidget *paintedWidget) const
{
    if (option->levelOfDetailFromTransform(painter->worldTransform()) < 0.1)
    {
        return false;
    }
    // TODO: remove check once all views inherit from AbstractLayoutView
    LayoutView *view = paintedView(paintedWidget);
    if (view != nullptr && !view->shouldPaintItem(this))
    {
        return false;
    }
    return true;
}

QColor GraphicsItem::color(QWidget *paintedWidget) const
{
    // TODO: remove check once all views inherit from AbstractLayoutView
    LayoutView *view = paintedView(paintedWidget);
    if (view != nullptr)
    {
        return paintedView(paintedWidget)->colorForItem(this);
    }
    return Qt::red;
}

bool GraphicsItem::fromJson(QString *errorString, const QJsonObject &jsonObject)
{
    QPointF pos;
    if (!Json::toPoint(errorString, jsonObject.value(J_POSITION), pos))
    {
        return false;
    }
    int layer;
    if (!Json::toInt(errorString, jsonObject.value(J_LAYER), layer))
    {
        return false;
    }
    bool locked = false;
    if (jsonObject.contains(J_LOCKED) && !Json::toBool(errorString, jsonObject.value(J_LOCKED), locked))
    {
        return false;
    }
    qreal rot = 0;
    if (jsonObject.contains(J_ROTATION)
            && !Json::toReal(errorString, jsonObject.value(J_ROTATION), rot))
    {
        return false;
    }
    setPos(pos);
    setLayer(layer);
    setLocked(locked);
    setRotation(rot);
    return true;
}

void GraphicsItem::toJson(QJsonObject &jsonObject) const
{
    jsonObject.insert(J_POSITION, Json::fromPoint(pos()));
    jsonObject.insert(J_LAYER, QJsonValue(m_layer));
    jsonObject.insert(J_LOCKED, QJsonValue(m_isLocked));
    jsonObject.insert(J_ROTATION, QJsonValue(rotation()));
}
