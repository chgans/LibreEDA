#include "item/wireitem.h"
#include "handle/handle.h"

#include <QGraphicsPathItem>

using namespace SymbolEditor;

WireItem::WireItem(Item *parent):
    Item(parent)
{
}

WireItem::~WireItem()
{

}

QRectF WireItem::boundingRect() const
{
    if (m_boundingRect.isNull())
    {
        qreal pw = pen().style() == Qt::NoPen ? qreal(0) : pen().widthF();
        if (pw == 0.0)
        {
            m_boundingRect = m_path.boundingRect();
        }
        else
        {
            m_boundingRect = shape().controlPointRect();
        }
    }
    return m_boundingRect;
}

QPainterPath WireItem::shape() const
{
    QPainterPath path = shapeFromPath(m_path, pen());
    return path;
}

void WireItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                             QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(pen());
    painter->setBrush(QBrush());
    painter->drawPath(m_path);
}

Item *WireItem::clone()
{
    WireItem *item = new WireItem();
    item->setPoints(points());
    cloneTo(item);
    return item;
}

QList<QPointF> WireItem::points() const
{
    QList<QPointF> result;
    for (int i = 0; i < m_path.elementCount(); i++)
    {
        result.append(QPointF(m_path.elementAt(i).x,
                              m_path.elementAt(i).y));
    }
    return result;
}

void WireItem::addPoint(const QPointF &pos)
{
    addRegularHandle(m_path.elementCount(), MoveHandleRole, CircularHandleShape, pos);

    prepareGeometryChange();
    if (m_path.elementCount() == 0)
    {
        m_path.moveTo(pos);
    }
    else
    {
        m_path.lineTo(pos);
    }
    m_boundingRect = QRectF();
    update();

    emit pointsChanged();
}

void WireItem::movePoint(int idx, const QPointF &pos)
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

void WireItem::setPoints(QList<QPointF> points)
{
    removeAllHandles();
    m_path = QPainterPath();

    for (int i = 0; i < points.count(); i++)
    {
        addRegularHandle(i, MoveHandleRole, CircularHandleShape, points[i]);
    }

    prepareGeometryChange();
    m_path = QPainterPath();
    if (!m_idToHandle.isEmpty())
    {
        m_path.moveTo(m_idToHandle[0]->pos());
        for (int i = 1; i < m_idToHandle.count(); i++)
        {
            m_path.lineTo(m_idToHandle[i]->pos());
        }
    }
    m_boundingRect = QRectF();
    update();

    emit pointsChanged();
}

void WireItem::itemNotification(IObservableItem *item)
{
    Handle *handle = static_cast<Handle *>(item);
    int idx = m_handleToId[handle];

    prepareGeometryChange();
    m_path.setElementPositionAt(idx, handle->pos().x(), handle->pos().y());
    m_boundingRect = QRectF();
    update();

    emit pointsChanged();
}


QVariant WireItem::itemChange(QGraphicsItem::GraphicsItemChange change,
                                      const QVariant &value)
{
    if (change == QGraphicsItem::ItemSelectedHasChanged)
    {
        for (Handle *handle : m_idToHandle)
        {
            handle->setVisible(isSelected());
        }
    }
    return value;
}
