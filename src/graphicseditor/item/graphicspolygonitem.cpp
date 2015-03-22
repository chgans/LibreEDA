#include "graphicspolygonitem.h"

GraphicsPolygonItem::GraphicsPolygonItem(GraphicsObject *parent):
    GraphicsObject(parent), IGraphicsItemObserver(),
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
        return;

    m_fillRule = fillRule;
    update();
    emit fillRuleChanged(fillRule);
}

void GraphicsPolygonItem::setPolygon(QPolygonF polygon)
{
    if (m_polygon == polygon)
        return;

    for (int i = 0; i < m_handles.count(); i++)
        removeHandle(i);

    prepareGeometryChange();
    m_polygon = polygon;
    foreach (const QPointF &pos, polygon)
        addHandle(pos);
    m_boundingRect = QRectF();
    update();

    emit polygonChanged(polygon);
}

void GraphicsPolygonItem::addPoint(const QPointF &pos)
{
    m_polygon.append(pos);
    addHandle(pos);
    handleToPolygon();

    emit polygonChanged(m_polygon);
}

void GraphicsPolygonItem::movePoint(int idx, const QPointF &pos)
{
    blockItemNotification();
    m_handles[idx]->setPos(pos);
    unblockItemNotification();
    handleToPolygon();

    emit polygonChanged(m_polygon);
}

GraphicsHandle *GraphicsPolygonItem::addHandle(const QPointF &pos)
{
    GraphicsHandle *handle = new GraphicsHandle(this);
    handle->setHandleShape(CircularHandleShape);
    handle->setRole(MoveHandleRole);
    handle->setPos(pos);
    m_handles.append(handle);
    blockItemNotification();
    addObservedItem(handle);
    unblockItemNotification();
    return handle;
}

void GraphicsPolygonItem::removeHandle(int idx)
{
    Q_ASSERT(idx < m_handles.count());
    GraphicsHandle *handle = m_handles[idx];

    blockItemNotification();
    removeObservedItem(handle);
    unblockItemNotification();
    handle->setParentItem(nullptr);
    m_handles.removeAt(idx);
    delete handle;
}

void GraphicsPolygonItem::handleToPolygon()
{
    prepareGeometryChange();
    m_polygon = QPolygonF();
    for (int i = 0; i < m_handles.count(); i++) {
        m_polygon.append(m_handles[i]->pos());
    }
    m_boundingRect = QRectF();
    update();
}

void GraphicsPolygonItem::polygonToHandle()
{
    blockItemNotification();
    for (int i = 0; i < m_polygon.count(); i++) {
        m_handles[i]->setPos(m_polygon[i].x(),
                             m_polygon[i].y());
    }
    unblockItemNotification();
}

QRectF GraphicsPolygonItem::boundingRect() const
{
    if (m_boundingRect.isNull()) {
        qreal pw = pen().style() == Qt::NoPen ? qreal(0) : pen().widthF();
        if (pw == 0.0)
            m_boundingRect = m_polygon.boundingRect();
        else
            m_boundingRect = shape().controlPointRect();
    }
    return m_boundingRect;
}

QPainterPath GraphicsPolygonItem::shape() const
{
    QPainterPath path;
    path.addPolygon(polygon());
    return shapeFromPath(path, pen());
}

void GraphicsPolygonItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(pen());
    painter->setBrush(brush());
    painter->drawPolygon(polygon(), fillRule());
}

GraphicsObject *GraphicsPolygonItem::clone()
{
    GraphicsPolygonItem *item = new GraphicsPolygonItem();
    item->setPolygon(polygon());
    item->setFillRule(fillRule());
    return item;
}


QVariant GraphicsPolygonItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemSelectedHasChanged) {
        foreach (GraphicsHandle *handle, m_handles) {
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
