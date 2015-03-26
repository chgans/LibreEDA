#include "graphicsrectitem.h"
#include "graphicshandle.h"

#include <QBrush>
#include <QPen>
#include <QRectF>
#include <QPainter>

#include <QDebug>

// TODO: forbid objects to have write access to handles

GraphicsRectItem::GraphicsRectItem(GraphicsObject *parent):
    GraphicsObject(parent), m_rect(QRectF(0, 0, 0, 0))
{
    addHandle(TopLeft, FDiagSizeHandleRole, CircularHandleShape);
    addHandle(Top, VSizeHandleRole, DiamondedHandleShape);
    addHandle(TopRight, BDiagSizeHandleRole, CircularHandleShape);
    addHandle(Right, HSizeHandleRole, DiamondedHandleShape);
    addHandle(BottomRight, FDiagSizeHandleRole, CircularHandleShape);
    addHandle(Bottom, VSizeHandleRole, DiamondedHandleShape);
    addHandle(BottomLeft, BDiagSizeHandleRole, CircularHandleShape);
    addHandle(Left, HSizeHandleRole, DiamondedHandleShape);
}

GraphicsRectItem::~GraphicsRectItem()
{

}

QRectF GraphicsRectItem::rect() const
{
    return m_rect;
}

void GraphicsRectItem::setRect(const QRectF &rect)
{
    prepareGeometryChange();
    m_rect = rect;
    m_boundingRect = QRectF();
    update();

    blockItemNotification();
    qreal midX = m_rect.right()-m_rect.width()/2.0;
    qreal midY = m_rect.bottom()-m_rect.height()/2.0;
    m_idToHandle[TopLeft]->setPos(m_rect.topLeft());
    m_idToHandle[Top]->setPos(QPointF(midX, m_rect.top()));
    m_idToHandle[TopRight]->setPos(m_rect.topRight());
    m_idToHandle[Right]->setPos(QPointF(m_rect.right(), midY));
    m_idToHandle[BottomRight]->setPos(m_rect.bottomRight());
    m_idToHandle[Bottom]->setPos(QPointF(midX, m_rect.bottom()));
    m_idToHandle[BottomLeft]->setPos(m_rect.bottomLeft());
    m_idToHandle[Left]->setPos(QPointF(m_rect.left(), midY));
    unblockItemNotification();
}

GraphicsObject *GraphicsRectItem::clone()
{
    GraphicsRectItem *item = new GraphicsRectItem();
    GraphicsObject::cloneTo(item);
    item->setRect(rect());
    return item;
}

void GraphicsRectItem::itemNotification(IGraphicsObservableItem *item)
{
    GraphicsHandle *handle = dynamic_cast<GraphicsHandle*>(item);
    Q_ASSERT(handle);

    HandleId id = HandleId(m_handleToId[handle]);
    QRectF r = m_rect;
    switch (id) {
    case TopLeft:
        r.setTopLeft(handle->pos());
        break;
    case Top:
        r.setTop(handle->pos().y());
        break;
    case TopRight:
        r.setTopRight(handle->pos());
        break;
    case Right:
        r.setRight(handle->pos().x());
        break;
    case BottomRight:
        r.setBottomRight(handle->pos());
        break;
    case Bottom:
        r.setBottom(handle->pos().y());
        break;
    case BottomLeft:
        r.setBottomLeft(handle->pos());
        break;
    case Left:
        r.setLeft(handle->pos().x());
        break;
    default:
        break;
    }
    setRect(r);
}

QRectF GraphicsRectItem::boundingRect() const
{
    if (m_boundingRect.isNull()) {
        qreal halfpw = pen().style() == Qt::NoPen ? qreal(0) : pen().widthF() / 2;
        m_boundingRect = m_rect;
        if (halfpw > 0.0)
            m_boundingRect.adjust(-halfpw, -halfpw, halfpw, halfpw);
    }
    return m_boundingRect;

}

QPainterPath GraphicsRectItem::shape() const
{
    QPainterPath path;
    path.addRect(m_rect);
    return shapeFromPath(path, pen());
}

void GraphicsRectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(pen());
    painter->setBrush(brush());
    painter->drawRect(m_rect);
}

QVariant GraphicsRectItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemSelectedHasChanged) {
        foreach (QGraphicsItem *h, childItems()) {
            h->setVisible(isSelected());
        }
    }
    return value;
}
