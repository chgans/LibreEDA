#include "graphicscircleitem.h"
#include "core/json.h"

#include <QStyleOptionGraphicsItem>

GraphicsCircleItem::GraphicsCircleItem(SchItem *parent):
    SchItem(parent)
{
    addRegularHandle(RadiusHandle, MoveHandleRole, DiamondedHandleShape);
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

SchItem *GraphicsCircleItem::clone()
{
    GraphicsCircleItem *item = new GraphicsCircleItem();
    SchItem::cloneTo(item);
    item->setRadius(radius());
    return item;
}

void GraphicsCircleItem::itemNotification(IGraphicsObservableItem *item)
{
    AbstractGraphicsHandle *handle = static_cast<AbstractGraphicsHandle *>(item);

    setRadius(qAbs(handle->pos().x()));
}

bool GraphicsCircleItem::fromJson(QString *errorString, const QJsonObject &jsonObject)
{
    if (!SchItem::fromJson(errorString, jsonObject))
        return false;
    if (!SchItem::fromJson(errorString, jsonObject))
        return false;
    if (!jsonObject.contains("radius")) {
        *errorString = "Circle item: missing radius";
        return false;
    }
    qreal radius;
    if (!Json::toReal(errorString, jsonObject.value("radius"), radius)) {
        return false;
    }
    setRadius(radius);
    return true;
}

void GraphicsCircleItem::toJson(QJsonObject &jsonObject) const
{
    SchItem::toJson(jsonObject);
    jsonObject.insert("radius", Json::fromReal(radius()));
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
        foreach (AbstractGraphicsHandle *handle, m_handleToId.keys()) {
            handle->setVisible(isSelected());
        }
    }
    return value;
}

