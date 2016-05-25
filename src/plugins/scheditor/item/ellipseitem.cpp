#include "item/ellipseitem.h"

#include <QStyleOptionGraphicsItem>
#include <qmath.h>

using namespace SymbolEditor;

EllipseItem::EllipseItem(Item *parent):
    Item(parent),
    m_xRadius(0.0f), m_yRadius(0.0f)
{
    addRegularHandle(XRadiusHandle, MoveHandleRole, DiamondedHandleShape);
    addRegularHandle(YRadiusHandle, MoveHandleRole, DiamondedHandleShape);
}

EllipseItem::~EllipseItem()
{

}

qreal EllipseItem::xRadius() const
{
    return m_xRadius;
}

qreal EllipseItem::yRadius() const
{
    return m_yRadius;
}

void EllipseItem::setXRadius(qreal xRadius)
{
    if (qFuzzyCompare(m_xRadius, xRadius))
    {
        return;
    }

    prepareGeometryChange();
    m_xRadius = xRadius;
    m_boundingRect = QRectF();
    update();

    blockItemNotification();
    m_idToHandle[XRadiusHandle]->setPos(m_xRadius, 0);
    unblockItemNotification();

    emit xRadiusChanged(xRadius);
}

void EllipseItem::setYRadius(qreal yRadius)
{
    if (qFuzzyCompare(m_yRadius, yRadius))
    {
        return;
    }

    prepareGeometryChange();
    m_yRadius = yRadius;
    m_boundingRect = QRectF();
    update();

    blockItemNotification();
    m_idToHandle[YRadiusHandle]->setPos(0, m_yRadius);
    unblockItemNotification();

    emit yRadiusChanged(yRadius);
}

QPointF EllipseItem::pointAt(int angle) const
{
    qreal theta = qDegreesToRadians(angle / 16.0);
    return QPointF(m_xRadius * qCos(theta), -m_yRadius * qSin(theta));
}

qreal EllipseItem::angleAt(const QPointF &pos) const
{
    QLineF vector(QPointF(0, 0), QPointF(pos.x() / xRadius(), pos.y() / m_yRadius));
    return int(16 * vector.angle());
}

QRectF EllipseItem::boundingRect() const
{
    if (m_boundingRect.isNull())
    {
        qreal pw = pen().style() == Qt::NoPen ? qreal(0) : pen().widthF();
        if (pw == 0.0 /*&& m_spanAngle == 360 * 16*/)
        {
            m_boundingRect.setWidth(2 * xRadius());
            m_boundingRect.setHeight(2 * yRadius());
        }
        else
        {
            m_boundingRect = shape().controlPointRect();
        }
    }
    return m_boundingRect;
}

QPainterPath EllipseItem::shape() const
{
    QPainterPath path;
    path.addEllipse(QPointF(0, 0), m_xRadius, m_yRadius);
    return shapeFromPath(path, pen());
}

void EllipseItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                                QWidget *widget)
{
    Q_UNUSED(widget);

    painter->setPen(pen());
    painter->setBrush(brush());
    painter->setClipRect(option->exposedRect);
    painter->drawEllipse(QPointF(0, 0), xRadius(), yRadius());
}

Item *EllipseItem::clone()
{
    EllipseItem *item = new EllipseItem();
    item->setXRadius(xRadius());
    item->setYRadius(yRadius());
    Item::cloneTo(item);
    return item;
}

QList<QPointF> EllipseItem::endPoints() const
{
    return QList<QPointF>() << QPointF(m_xRadius, 0);
}

QList<QPointF> EllipseItem::midPoints() const
{
    return QList<QPointF>();
}

QList<QPointF> EllipseItem::centerPoints() const
{
    return QList<QPointF>() << QPointF(0, 0);
}

QList<QPointF> EllipseItem::nearestPoints(QPointF pos) const
{
    qreal theta = angleAt(pos);
    return QList<QPointF>() << pointAt(theta);
}

QVariant EllipseItem::itemChange(QGraphicsItem::GraphicsItemChange change,
                                         const QVariant &value)
{
    if (change == QGraphicsItem::ItemSelectedHasChanged)
    {
        for (Handle *handle : m_handleToId.keys())
        {
            handle->setVisible(isSelected());
        }
    }
    return value;
}

void EllipseItem::itemNotification(IObservableItem *item)
{
    Handle *handle = static_cast<Handle *>(item);
    if (handle == m_idToHandle[XRadiusHandle])
    {
        setXRadius(qAbs(handle->pos().x()));
    }
    else
    {
        setYRadius(qAbs(handle->pos().y()));
    }
}
