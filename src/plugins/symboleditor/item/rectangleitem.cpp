#include "item/rectangleitem.h"
#include "handle/handle.h"

#include <QBrush>
#include <QPen>
#include <QRectF>
#include <QPainter>

// TODO: forbid objects to have write access to handles
// TODO: Like Inkscape, if one round handle is in the corner, but not the other,
//       then use same roundness for both
// FIXME: Roundness handles are missplaced when the rect is not normal

using namespace SymbolEditor;

RectangleItem::RectangleItem(Item *parent):
    Item(parent), m_rect(QRectF(0, 0, 0, 0)),
    m_xRoundness(33), m_yRoundness(66)
{
    addRegularHandle(TopLeft, FDiagSizeHandleRole, DiamondedHandleShape);
    addRegularHandle(BottomRight, FDiagSizeHandleRole, DiamondedHandleShape);
    addRegularHandle(Bottom, VSizeHandleRole, DiamondedHandleShape);
    addRegularHandle(Left, HSizeHandleRole, DiamondedHandleShape);
    addRegularHandle(XRoundness, HSizeHandleRole, CircularHandleShape);
    addRegularHandle(YRoundness, VSizeHandleRole, CircularHandleShape);
    updateSizeHandles();
    updateRoundnessHandles();
}

RectangleItem::~RectangleItem()
{

}

QRectF RectangleItem::rect() const
{
    return m_rect;
}

void RectangleItem::setRect(const QRectF &rect)
{
    prepareGeometryChange();
    m_rect = rect;
    m_boundingRect = QRectF();
    update();
    updateSizeHandles();
    updateRoundnessHandles();
}

void RectangleItem::setRoundness(qreal xRoundness, qreal yRoundness)
{
    setXRoundness(xRoundness);
    setYRoundness(yRoundness);
}

qreal RectangleItem::xRoundness() const
{
    return m_xRoundness;
}

void RectangleItem::setXRoundness(qreal roundness)
{
    qreal xRoundness = qMin(qMax(roundness, 0.0), 100.0);
    if (qFuzzyCompare(xRoundness, m_xRoundness))
    {
        updateRoundnessHandles();
        return;
    }

    prepareGeometryChange();
    m_xRoundness = xRoundness;
    update();
    updateRoundnessHandles();
}

qreal RectangleItem::yRoundness() const
{
    return m_yRoundness;
}

void RectangleItem::setYRoundness(qreal roundness)
{
    qreal yRoundness = qMin(qMax(roundness, 0.0), 100.0);
    if (qFuzzyCompare(yRoundness, m_yRoundness))
    {
        updateRoundnessHandles();
        return;
    }

    prepareGeometryChange();
    m_yRoundness = yRoundness;
    update();
    updateRoundnessHandles();
}

void RectangleItem::updateSizeHandles()
{
    blockItemNotification();
    qreal midX = m_rect.right() - m_rect.width() / 2.0;
    qreal midY = m_rect.bottom() - m_rect.height() / 2.0;
    m_idToHandle[TopLeft]->setPos(m_rect.topLeft());
    m_idToHandle[BottomRight]->setPos(m_rect.bottomRight());
    m_idToHandle[Bottom]->setPos(QPointF(midX, m_rect.bottom()));
    m_idToHandle[Left]->setPos(QPointF(m_rect.left(), midY));
    if (m_rect == m_rect.normalized())
    {
        m_idToHandle[TopLeft]->setHandleRole(FDiagSizeHandleRole);
        m_idToHandle[BottomRight]->setHandleRole(FDiagSizeHandleRole);
    }
    else
    {
        m_idToHandle[TopLeft]->setHandleRole(BDiagSizeHandleRole);
        m_idToHandle[BottomRight]->setHandleRole(BDiagSizeHandleRole);
    }
    unblockItemNotification();
}

void RectangleItem::updateRoundnessHandles()
{
    blockItemNotification();
    QRectF rect = m_rect.normalized();
    QPointF xPos(rect.right() - (m_xRoundness / 100.0) * (m_rect.width()  / 2.0), rect.top());
    m_idToHandle[XRoundness]->setPos(xPos);
    QPointF yPos(rect.right(), rect.top() + (m_yRoundness / 100.0) * (m_rect.height() / 2.0));
    m_idToHandle[YRoundness]->setPos(yPos);
    unblockItemNotification();
}

Item *RectangleItem::clone()
{
    RectangleItem *item = new RectangleItem();
    Item::cloneTo(item);
    item->setRect(rect());
    item->setXRoundness(m_xRoundness);
    item->setYRoundness(m_yRoundness);
    return item;
}

void RectangleItem::itemNotification(IObservableItem *item)
{
    Handle *handle = dynamic_cast<Handle *>(item);
    Q_ASSERT(handle);

    int id = handle->handleId();
    QRectF rect = m_rect;
    QRectF normedRect = m_rect.normalized();
    switch (id)
    {
        case XRoundness:
            setXRoundness(100.0 * (normedRect.right() - handle->pos().x()) / (normedRect.width() / 2.0));
            return;
        case YRoundness:
            setYRoundness(100.0 * (handle->pos().y() - normedRect.top()) / (normedRect.height() / 2.0));
            return;
        case TopLeft:
            rect.setTopLeft(handle->pos());
            break;
        case BottomRight:
            rect.setBottomRight(handle->pos());
            break;
        case Bottom:
            rect.setBottom(handle->pos().y());
            break;
        case Left:
            rect.setLeft(handle->pos().x());
            break;
        default:
            Q_ASSERT(false);
            return;
    }
    setRect(rect);
}

QList<QPointF> RectangleItem::endPoints() const
{
    return QList<QPointF>() << m_rect.topLeft()
           << m_rect.topRight()
           << m_rect.bottomRight()
           << m_rect.bottomLeft();
}

QList<QPointF> RectangleItem::midPoints() const
{

    return QList<QPointF>() << QPointF(m_rect.center().x(), m_rect.top())
           << QPointF(m_rect.right(), m_rect.center().y())
           << QPointF(m_rect.center().x(), m_rect.bottom())
           << QPointF(m_rect.left(), m_rect.center().y());
}

QList<QPointF> RectangleItem::centerPoints() const
{
    return midPoints();
}

QList<QPointF> RectangleItem::nearestPoints(QPointF pos) const
{
    QRectF rect = m_rect.normalized();
    QList<QLineF> lines;
    lines << QLineF(rect.topLeft(),     rect.topRight())
          << QLineF(rect.topRight(),    rect.bottomRight())
          << QLineF(rect.bottomRight(), rect.bottomLeft())
          << QLineF(rect.bottomLeft(),  rect.topLeft());

    QList<QPointF> xPoints;
    QPointF xPoint;
    for (const QLineF &line : lines)
    {
        QLineF::IntersectType xType = line.intersect(line.normalVector().translated(pos), &xPoint);
        if (xType == QLineF::BoundedIntersection)
        {
            xPoints << xPoint;
        }
    }

    return xPoints;
}

QRectF RectangleItem::boundingRect() const
{
    if (m_boundingRect.isNull())
    {
        qreal halfpw = pen().style() == Qt::NoPen ? qreal(0) : pen().widthF() / 2;
        m_boundingRect = m_rect;
        if (halfpw > 0.0)
        {
            m_boundingRect.adjust(-halfpw, -halfpw, halfpw, halfpw);
        }
    }
    return m_boundingRect;

}

QPainterPath RectangleItem::shape() const
{
    QPainterPath path;
    path.addRoundedRect(m_rect, m_xRoundness, m_yRoundness, Qt::RelativeSize);
    return shapeFromPath(path, pen());
}

void RectangleItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                             QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(pen());
    painter->setBrush(brush());
    painter->drawRoundedRect(m_rect, m_xRoundness, m_yRoundness, Qt::RelativeSize);
}

QVariant RectangleItem::itemChange(QGraphicsItem::GraphicsItemChange change,
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


bool RectangleItem::contains(const QPointF &point) const
{
    // FIXME: Needed to avoid slowing down the view: default impl of contains() call
    // shape() and QPainterPath implement the rounded corner with bezier curves,
    // which is CPU intensive. Relying on boundingRect() is super fast, but quite
    // inacurate, find and implement a good trade off b/w speed and precision
    return boundingRect().contains(point);
}
