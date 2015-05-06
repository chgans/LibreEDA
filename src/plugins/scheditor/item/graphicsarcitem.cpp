#include "graphicsarcitem.h"
#include "core/json.h"

#include <qmath.h>
#include <QStyleOptionGraphicsItem>
#include <QDebug>

GraphicsArcItem::GraphicsArcItem(SchItem *parent):
    SchItem(parent),
    m_startAngle(0.0), m_spanAngle(360.0*16)
{
    addHandles();
}

GraphicsArcItem::GraphicsArcItem(const QRectF &rect, SchItem *parent):
    SchItem(parent),
    m_startAngle(0.0), m_spanAngle(360.0*16)
{
    setRect(rect);
    addHandles();
}

GraphicsArcItem::GraphicsArcItem(qreal x, qreal y, qreal w, qreal h, SchItem *parent):
    SchItem(parent),
    m_startAngle(0.0), m_spanAngle(360.0*16)
{
    setRect(x, y, w, h);
    addHandles();
}

GraphicsArcItem::~GraphicsArcItem()
{

}

QRectF GraphicsArcItem::rect() const
{
    return m_rect;
}

int GraphicsArcItem::startAngle() const
{
    return m_startAngle;
}

int GraphicsArcItem::spanAngle() const
{
    return m_spanAngle;
}

void GraphicsArcItem::setRect(const QRectF &rect)
{
    if (rect == m_rect)
        return;
    prepareGeometryChange();
    m_boundingRect = QRectF();
    m_rect = rect;
    update();
    updateHandles();
}

void GraphicsArcItem::setStartAngle(int angle)
{
    if (angle == m_startAngle)
        return;
    qDebug() << "Start angle" << angle/16.0;
    prepareGeometryChange();
    m_boundingRect = QRectF();
    m_startAngle = angle;
    update();
    updateHandles();
}

void GraphicsArcItem::setSpanAngle(int angle)
{
    if (angle == m_spanAngle)
        return;
    qDebug() << "Span angle" << angle/16.0;
    prepareGeometryChange();
    m_boundingRect = QRectF();
    m_spanAngle = angle;
    update();
    updateHandles();
}

void GraphicsArcItem::addHandles()
{
    addRegularHandle(RectHandle, MoveHandleRole, CircularHandleShape);
    addRegularHandle(StartAngleHandle, MoveHandleRole, DiamondedHandleShape);
    addRegularHandle(SpanAngleHandle, MoveHandleRole, SquaredHandleShape);
}

void GraphicsArcItem::updateHandles()
{
    blockItemNotification();
    handleAt(RectHandle)->setPos(m_rect.bottomRight());
    handleAt(StartAngleHandle)->setPos(pointAt(m_startAngle));
    handleAt(SpanAngleHandle)->setPos(pointAt(m_startAngle + m_spanAngle));
    unblockItemNotification();
}

QPointF GraphicsArcItem::pointAt(int angle)
{
    qreal theta = qDegreesToRadians(-angle / 16.0f);
    qreal aRadius = m_rect.width() / 2.0;
    qreal bRadius = m_rect.height() / 2.0;
    QPointF point(aRadius * qCos(theta), bRadius * qSin(theta));
    return  point + m_rect.center();
}

int GraphicsArcItem::angleAt(const QPointF &pos)
{
    QPointF point = pos - m_rect.center();
    point.rx() /= m_rect.width();
    point.ry() /= m_rect.height();
    QLineF vector(QPointF(0, 0), point);
    return vector.angle() * 16;
}

QRectF GraphicsArcItem::boundingRect() const
{
    if (m_boundingRect.isNull()) {
        qreal pw = pen().style() == Qt::NoPen ? qreal(0) : pen().widthF();
        if (pw == 0.0 && m_spanAngle == 360 * 16)
            m_boundingRect = m_rect;
        else
            m_boundingRect = shape().controlPointRect();
    }
    return m_boundingRect;
}

QPainterPath GraphicsArcItem::shape() const
{
    QPainterPath path;
    if (m_rect.isNull())
        return path;
    if (m_spanAngle != 360 * 16) {
        path.moveTo(m_rect.center());
        path.arcTo(m_rect, m_startAngle / 16.0, m_spanAngle / 16.0);
    } else {
        path.addEllipse(m_rect);
    }

    return shapeFromPath(path, pen());
}

void GraphicsArcItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(pen());
    painter->setBrush(brush());
    qDebug() << m_startAngle/16.0 << m_spanAngle/16.0;
    if ((m_spanAngle != 0) && (qAbs(m_spanAngle) % (360 * 16) == 0))
        painter->drawEllipse(m_rect);
    else
        painter->drawPie(m_rect, m_startAngle, m_spanAngle);

    if (option->state & QStyle::State_Selected) {
        painter->setBrush(Qt::NoBrush);
        painter->setPen(QPen(QBrush(Qt::red), 0, Qt::DashLine));
        painter->drawRect(m_rect);
    }
}

QVariant GraphicsArcItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemSelectedHasChanged) {
        for (int i = 0; i < handleCount(); i++)
            handleAt(i)->setVisible(isSelected());
    }
    return value;
}

void GraphicsArcItem::itemNotification(IGraphicsObservableItem *item)
{
    AbstractGraphicsHandle *handle = static_cast<AbstractGraphicsHandle *>(item);
    QRectF rect = m_rect;
    int angle = angleAt(handle->pos());
    switch (handle->handleId()) {
    case RectHandle:
        rect.setBottomRight(handle->pos());
        setRect(rect);
        break;
    case StartAngleHandle:
        setStartAngle(angle);
        break;
    case SpanAngleHandle:
        angle = angle - m_startAngle;
        if (angle < 0)
            angle += 360*16;
        setSpanAngle(angle);
        break;
    default:
        break;
    }
}

SchItem *GraphicsArcItem::clone()
{
    GraphicsArcItem *item = new GraphicsArcItem();
    item->setRect(m_rect);
    item->setStartAngle(m_startAngle);
    item->setSpanAngle(m_spanAngle);
    SchItem::cloneTo(item);
    return item;
}

bool GraphicsArcItem::fromJson(QString *errorString, const QJsonObject &jsonObject)
{
    if (!SchItem::fromJson(errorString, jsonObject))
        return false;

    if (!jsonObject.contains("xRadius") || !jsonObject.contains("yRadius")) {
        *errorString = "Arc item: missing X and/or Y radius";
        return false;
    }
    qreal xradius;
    if (!Json::toReal(errorString, jsonObject.value("xRadius"), xradius))
        return false;
    qreal yradius;
    if (!Json::toReal(errorString, jsonObject.value("yRadius"), yradius))
        return false;

    if (!jsonObject.contains("startAngle") || !jsonObject.contains("spanAngle")) {
        *errorString = "Arc item: missing start/span angle";
        return false;
    }
    int start;
    if (!Json::toInt(errorString, jsonObject.value("startAngle"), start))
        return false;
    int span;
    if (!Json::toInt(errorString, jsonObject.value("spanAngle"), span))
        return false;

    setRect(0, 0, xradius, yradius);
    setStartAngle(start);
    setSpanAngle(span);
    return true;
}

void GraphicsArcItem::toJson(QJsonObject &jsonObject) const
{
    SchItem::toJson(jsonObject);
    jsonObject.insert("xRadius", Json::fromReal(m_rect.width()));
    jsonObject.insert("yRadius", Json::fromReal(m_rect.height()));
    jsonObject.insert("startAngle", Json::fromReal(m_startAngle));
    jsonObject.insert("spanAngle", Json::fromReal(m_spanAngle));
}
