#include "item/graphicswireitem.h"
#include "handle/abstractgraphicshandle.h"
#include "schscene.h"
#include "core/json.h"

#include <QGraphicsPathItem>

#include <QDebug>

const QString GraphicsWireItem::J_POINTS = QStringLiteral("points");

GraphicsWireItem::GraphicsWireItem(SchItem *parent):
    SchItem(parent)
{
}

GraphicsWireItem::~GraphicsWireItem()
{

}

QRectF GraphicsWireItem::boundingRect() const
{
    if (m_boundingRect.isNull()) {
        qreal pw = pen().style() == Qt::NoPen ? qreal(0) : pen().widthF();
        if (pw == 0.0)
            m_boundingRect = m_path.boundingRect();
        else
            m_boundingRect = shape().controlPointRect();
    }
    return m_boundingRect;
}

QPainterPath GraphicsWireItem::shape() const
{
    QPainterPath path = shapeFromPath(m_path, pen());
    return path;
}

void GraphicsWireItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(pen());
    painter->setBrush(QBrush());
    painter->drawPath(m_path);
}

SchItem *GraphicsWireItem::clone()
{
    GraphicsWireItem *item = new GraphicsWireItem();
    item->setPoints(points());
    cloneTo(item);
    return item;
}

bool GraphicsWireItem::fromJson(QString *errorString, const QJsonObject &jsonObject)
{
    if (!SchItem::fromJson(errorString, jsonObject))
        return false;
    if (!jsonObject.contains(J_POINTS)) {
        *errorString = "Wire item: missing point list";
        return false;
    }
    QList<QPointF> points;
    if (!Json::toPointList(errorString, jsonObject.value(J_POINTS), points))
        return false;
    setPoints(points);
    return true;
}

void GraphicsWireItem::toJson(QJsonObject &jsonObject) const
{
    SchItem::toJson(jsonObject);
    jsonObject.insert(J_POINTS, Json::fromPointList(points()));
}

QList<QPointF> GraphicsWireItem::points() const
{
    QList<QPointF> result;
    for (int i = 0; i < m_path.elementCount(); i++) {
        result.append(QPointF(m_path.elementAt(i).x,
                              m_path.elementAt(i).y));
    }
    return result;
}


void GraphicsWireItem::addPoint(const QPointF &pos)
{
    addRegularHandle(m_path.elementCount(), MoveHandleRole, CircularHandleShape, pos);

    prepareGeometryChange();
    if (m_path.elementCount() == 0)
        m_path.moveTo(pos);
    else
        m_path.lineTo(pos);
    m_boundingRect = QRectF();
    update();

    emit pointsChanged();
}

void GraphicsWireItem::movePoint(int idx, const QPointF &pos)
{
    blockItemNotification();
    m_idToHandle[idx]->setPos(pos);
    unblockItemNotification();

    prepareGeometryChange();
    m_path.setElementPositionAt(idx, pos.x(), pos.y());
    m_boundingRect = QRectF();
    update();

    emit pointsChanged();
}

void GraphicsWireItem::setPoints(QList<QPointF> points)
{
    removeAllHandles();
    m_path = QPainterPath();

    for (int i = 0; i < points.count(); i++)
        addRegularHandle(i, MoveHandleRole, CircularHandleShape, points[i]);

    prepareGeometryChange();
    m_path = QPainterPath();
    if (!m_idToHandle.isEmpty()) {
        m_path.moveTo(m_idToHandle[0]->pos());
        for (int i = 1; i < m_idToHandle.count(); i++) {
            m_path.lineTo(m_idToHandle[i]->pos());
        }
    }
    m_boundingRect = QRectF();
    update();

    emit pointsChanged();
}

void GraphicsWireItem::itemNotification(IGraphicsObservableItem *item)
{
    AbstractGraphicsHandle *handle = static_cast<AbstractGraphicsHandle*>(item);
    int idx = m_handleToId[handle];

    prepareGeometryChange();
    m_path.setElementPositionAt(idx, handle->pos().x(), handle->pos().y());
    m_boundingRect = QRectF();
    update();

    emit pointsChanged();
}


QVariant GraphicsWireItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemSelectedHasChanged) {
        foreach (AbstractGraphicsHandle *handle, m_idToHandle) {
            handle->setVisible(isSelected());
        }
    }
    return value;
}
