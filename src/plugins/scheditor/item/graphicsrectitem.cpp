#include "graphicsrectitem.h"
#include "abstractgraphicshandle.h"

#include <QBrush>
#include <QPen>
#include <QRectF>
#include <QPainter>

#include <QDebug>

// TODO: forbid objects to have write access to handles

GraphicsRectItem::GraphicsRectItem(SchItem *parent):
    SchItem(parent), m_rect(QRectF(0, 0, 0, 0))
{
    addRegularHandle(TopLeft, FDiagSizeHandleRole, CircularHandleShape);
    addRegularHandle(Top, VSizeHandleRole, DiamondedHandleShape);
    addRegularHandle(TopRight, BDiagSizeHandleRole, CircularHandleShape);
    addRegularHandle(Right, HSizeHandleRole, DiamondedHandleShape);
    addRegularHandle(BottomRight, FDiagSizeHandleRole, CircularHandleShape);
    addRegularHandle(Bottom, VSizeHandleRole, DiamondedHandleShape);
    addRegularHandle(BottomLeft, BDiagSizeHandleRole, CircularHandleShape);
    addRegularHandle(Left, HSizeHandleRole, DiamondedHandleShape);
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

SchItem *GraphicsRectItem::clone()
{
    GraphicsRectItem *item = new GraphicsRectItem();
    SchItem::cloneTo(item);
    item->setRect(rect());
    return item;
}

void GraphicsRectItem::itemNotification(IGraphicsObservableItem *item)
{
    AbstractGraphicsHandle *handle = dynamic_cast<AbstractGraphicsHandle*>(item);
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

bool GraphicsRectItem::fromJson(QString *errorString, const QJsonObject &jsonObject)
{
    return true;
}

void GraphicsRectItem::toJson(QJsonObject &jsonObject) const
{

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
        foreach (AbstractGraphicsHandle *handle, m_handleToId.keys()) {
            handle->setVisible(isSelected());
        }
    }
    return value;
}
