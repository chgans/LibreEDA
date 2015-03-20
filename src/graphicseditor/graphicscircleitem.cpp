#include "graphicscircleitem.h"

GraphicsCircleItem::GraphicsCircleItem(GraphicsObject *parent):
    GraphicsObject(parent)
{
    addHandle(CenterHandle, CircularHandleShape);
    addHandle(RadiusHandle, DiamondedHandleShape);
}

GraphicsCircleItem::~GraphicsCircleItem()
{

}

QRectF GraphicsCircleItem::rect() const
{
    return m_rect;
}

void GraphicsCircleItem::setRect(const QRectF &rect)
{
    m_rect = rect;
    markDirty();
}

QPen GraphicsCircleItem::pen() const
{
    return m_pen;
}

void GraphicsCircleItem::setPen(const QPen &pen)
{
    m_pen = pen;
    markDirty();
}

QBrush GraphicsCircleItem::brush() const
{
    return m_brush;
}

void GraphicsCircleItem::setBrush(const QBrush &brush)
{
    m_brush = brush;
}

void GraphicsCircleItem::addHandle(GraphicsCircleItem::HandleId handleId, GraphicsHandleShape shape)
{
    GraphicsHandle *handle = new GraphicsHandle(this);
    handle->setRole(MoveHandleRole);
    handle->setHandleShape(CircularHandleShape);
    handle->setPos(QPointF(0, 0));
    addObservedItem(handle);
    m_handleToId[handle] = handleId;
    m_idToHandle[handleId] = handle;
}

void GraphicsCircleItem::updateHandlesSilently()
{

}

void GraphicsCircleItem::updateGeometry() const
{
    QPainterPath path;
    path.addEllipse(m_rect);
    m_shape = path;

    qreal extra = pen().widthF()/2.0;
    m_boundingRect = m_rect.adjusted(-extra, -extra, +extra, +extra);

    m_dirty = false;
}

void GraphicsCircleItem::markDirty()
{
    prepareGeometryChange();
    m_dirty = true;
}

GraphicsObject *GraphicsCircleItem::clone()
{

}

void GraphicsCircleItem::itemNotification(IGraphicsObservableItem *item)
{
    GraphicsHandle *handle = dynamic_cast<GraphicsHandle*>(item);
    Q_ASSERT(handle);

    HandleId id = m_handleToId[handle];
    switch (id) {
    case CenterHandle:
        m_rect.setCenter(handle->pos());
        break;
    case RadiusHandle:
        break;
    default:
        break;
    }
    updateHandlesSilently();
    markDirty();
}
}

QRectF GraphicsCircleItem::boundingRect() const
{
    if (m_dirty)
        updateGeometry();
    return m_boundingRect;
}

QPainterPath GraphicsCircleItem::shape() const
{
    if (m_dirty)
        updateGeometry();
    return m_shape;
}

void GraphicsCircleItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(pen());
    painter->setBrush(brush());
    painter->drawEllipse(m_rect);
}

QVariant GraphicsCircleItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemSelectedHasChanged) {
        foreach (QGraphicsItem *h, childItems()) {
            h->setVisible(isSelected());
        }
    }
    return value;
}

