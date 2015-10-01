#include "item/graphicslineitem.h"
#include "handle/abstractgraphicshandle.h"
#include "core/json.h"

#include <QPainter>
#include <QPen>

#include <QDebug>

const QString GraphicsLineItem::J_POINTS = QStringLiteral("points");

GraphicsLineItem::GraphicsLineItem(SchItem *parent):
    SchItem(parent)
{
    addRegularHandle(P1Handle, MoveHandleRole, CircularHandleShape);
    addRegularHandle(P2Handle, MoveHandleRole, CircularHandleShape);
}

QLineF GraphicsLineItem::line() const
{
    return m_line;
}

void GraphicsLineItem::setLine(const QLineF &line)
{
    if (m_line == line)
        return;

    prepareGeometryChange();
    m_line = line;
    m_boundingRect = QRectF();
    update();

    blockItemNotification();
    m_idToHandle[P1Handle]->setPos(m_line.p1());
    m_idToHandle[P2Handle]->setPos(m_line.p2());
    unblockItemNotification();

    emit lineChanged();
}

SchItem *GraphicsLineItem::clone()
{
    GraphicsLineItem *item = new GraphicsLineItem();
    SchItem::cloneTo(item);
    item->setLine(line());
    return item;
}

bool GraphicsLineItem::fromJson(QString *errorString, const QJsonObject &jsonObject)
{
    if (!SchItem::fromJson(errorString, jsonObject))
        return false;
    if (!jsonObject.contains(J_POINTS)) {
        *errorString = "Line item: missing line points";
        return false;
    }
    QLineF line;
    if (!Json::toLine(errorString, jsonObject.value(J_POINTS), line))
        return false;
    setLine(line);
    return true;
}

void GraphicsLineItem::toJson(QJsonObject &jsonObject) const
{
    SchItem::toJson(jsonObject);
    jsonObject.insert(J_POINTS, Json::fromLine(line()));
}

QList<QPointF> GraphicsLineItem::endPoints() const
{
    return QList<QPointF>() << m_line.p1() << m_line.p2();
}

QList<QPointF> GraphicsLineItem::midPoints() const
{
    return QList<QPointF>() << m_line.pointAt(0.5);
}

QList<QPointF> GraphicsLineItem::centerPoints() const
{
    return QList<QPointF>() << m_line.pointAt(0.5);
}

QList<QPointF> GraphicsLineItem::nearestPoints(QPointF pos) const
{
    QPointF xPoint;
    QLineF::IntersectType xType = m_line.intersect(m_line.normalVector().translated(pos), &xPoint);
    if (xType == QLineF::BoundedIntersection)
        return QList<QPointF>() << xPoint;
    return QList<QPointF>();
}

void GraphicsLineItem::itemNotification(IGraphicsObservableItem *item)
{
    AbstractGraphicsHandle *handle = dynamic_cast<AbstractGraphicsHandle*>(item);
    Q_ASSERT(handle);

    QLineF l = m_line;
    if (handle == m_idToHandle[P1Handle]) {
        l.setP1(handle->pos());
    }
    else {
        l.setP2(handle->pos());
    }
    setLine(l);
}

QRectF GraphicsLineItem::boundingRect() const
{
    if (pen().widthF() == 0.0) {
        const qreal x1 = m_line.p1().x();
        const qreal x2 = m_line.p2().x();
        const qreal y1 = m_line.p1().y();
        const qreal y2 = m_line.p2().y();
        qreal lx = qMin(x1, x2);
        qreal rx = qMax(x1, x2);
        qreal ty = qMin(y1, y2);
        qreal by = qMax(y1, y2);
        m_boundingRect = QRectF(lx, ty, rx - lx, by - ty);
    }
    else
        m_boundingRect = shape().controlPointRect();

    return m_boundingRect;
}

QPainterPath GraphicsLineItem::shape() const
{
    QPainterPath path;
    if (m_line == QLineF())
        return path;

    path.moveTo(m_line.p1());
    path.lineTo(m_line.p2());

    return shapeFromPath(path, pen());
}

void GraphicsLineItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(pen());
    painter->drawLine(line());
}

QVariant GraphicsLineItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemSelectedHasChanged) {
        foreach (AbstractGraphicsHandle *handle, m_handleToId.keys()) {
            handle->setVisible(isSelected());
        }
    }
    return value;
}
