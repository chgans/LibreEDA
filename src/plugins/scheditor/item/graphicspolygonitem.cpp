#include "item/graphicspolygonitem.h"

GraphicsPolygonItem::GraphicsPolygonItem(SchItem *parent):
    SchItem(parent),
    m_fillRule(Qt::OddEvenFill)
{

}

GraphicsPolygonItem::~GraphicsPolygonItem()
{

}

Qt::FillRule GraphicsPolygonItem::fillRule() const
{
    return m_fillRule;
}

QPolygonF GraphicsPolygonItem::polygon() const
{
    return m_polygon;
}

void GraphicsPolygonItem::setFillRule(Qt::FillRule fillRule)
{
    if (m_fillRule == fillRule)
    {
        return;
    }

    m_fillRule = fillRule;
    update();
    emit fillRuleChanged(fillRule);
}

void GraphicsPolygonItem::setPolygon(QPolygonF polygon)
{
    if (m_polygon == polygon)
    {
        return;
    }

    removeAllHandles();

    prepareGeometryChange();
    m_polygon = polygon;
    for (int i = 0; i < polygon.count(); i++)
    {
        addRegularHandle(i, MoveHandleRole, CircularHandleShape, polygon[i]);
    }
    m_boundingRect = QRectF();
    update();

    emit polygonChanged(polygon);
}

void GraphicsPolygonItem::addPoint(const QPointF &pos)
{
    addRegularHandle(m_polygon.count(), MoveHandleRole, CircularHandleShape, pos);
    m_polygon.append(pos);
    handleToPolygon();

    emit polygonChanged(m_polygon);
}

void GraphicsPolygonItem::movePoint(int idx, const QPointF &pos)
{
    blockItemNotification();
    m_idToHandle[idx]->setPos(pos);
    unblockItemNotification();
    handleToPolygon();

    emit polygonChanged(m_polygon);
}

void GraphicsPolygonItem::handleToPolygon()
{
    prepareGeometryChange();
    m_polygon = QPolygonF();
    for (int i = 0; i < m_idToHandle.count(); i++)
    {
        m_polygon.append(m_idToHandle[i]->pos());
    }
    m_boundingRect = QRectF();
    update();
}

void GraphicsPolygonItem::polygonToHandle()
{
    blockItemNotification();
    for (int i = 0; i < m_polygon.count(); i++)
    {
        m_idToHandle[i]->setPos(m_polygon[i].x(),
                                m_polygon[i].y());
    }
    unblockItemNotification();
}

QRectF GraphicsPolygonItem::boundingRect() const
{
    if (m_boundingRect.isNull())
    {
        qreal pw = pen().style() == Qt::NoPen ? qreal(0) : pen().widthF();
        if (pw == 0.0)
        {
            m_boundingRect = m_polygon.boundingRect();
        }
        else
        {
            m_boundingRect = shape().controlPointRect();
        }
    }
    return m_boundingRect;
}

QPainterPath GraphicsPolygonItem::shape() const
{
    QPainterPath path;
    path.addPolygon(polygon());
    return shapeFromPath(path, pen());
}

void GraphicsPolygonItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                                QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(pen());
    painter->setBrush(brush());
    painter->drawPolygon(polygon(), fillRule());
}

SchItem *GraphicsPolygonItem::clone()
{
    GraphicsPolygonItem *item = new GraphicsPolygonItem();
    item->setPolygon(polygon());
    item->setFillRule(fillRule());
    SchItem::cloneTo(item);
    return item;
}

QVariant GraphicsPolygonItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemSelectedHasChanged)
    {
        for (AbstractGraphicsHandle *handle : m_idToHandle)
        {
            handle->setVisible(isSelected());
        }
    }
    return value;
}

void GraphicsPolygonItem::itemNotification(IGraphicsObservableItem *item)
{
    Q_UNUSED(item);
    handleToPolygon();
}
