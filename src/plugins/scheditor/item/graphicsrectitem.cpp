#include "graphicsrectitem.h"
#include "abstractgraphicshandle.h"
#include "core/json.h"

#include <QBrush>
#include <QPen>
#include <QRectF>
#include <QPainter>

#include <QDebug>

// TODO: forbid objects to have write access to handles
// TODO: Like Inkscape, if one round handle is in the corner, but not the other,
//       then use same roundness for both
// FIXME: Roundness handles are missplaced when the rect is not normal

const QString GraphicsRectItem::J_POINTS = QStringLiteral("points");

GraphicsRectItem::GraphicsRectItem(SchItem *parent):
    SchItem(parent), m_rect(QRectF(0, 0, 0, 0)),
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
    updateSizeHandles();
    updateRoundnessHandles();
}

void GraphicsRectItem::setRoundness(qreal xRoundness, qreal yRoundness)
{
    setXRoundness(xRoundness);
    setYRoundness(yRoundness);
}

qreal GraphicsRectItem::xRoundness() const
{
    return m_xRoundness;
}

void GraphicsRectItem::setXRoundness(qreal roundness)
{
    qDebug() << "X" << roundness;

    qreal xRoundness = qMin(qMax(roundness, 0.0), 100.0);
    if (qFuzzyCompare(xRoundness, m_xRoundness)) {
        updateRoundnessHandles();
        return;
    }

    qDebug() << "X" << xRoundness;
    prepareGeometryChange();
    m_xRoundness = xRoundness;
    update();
    updateRoundnessHandles();
}

qreal GraphicsRectItem::yRoundness() const
{
    return m_yRoundness;
}

void GraphicsRectItem::setYRoundness(qreal roundness)
{
    qDebug() << "Y" << roundness;

    qreal yRoundness = qMin(qMax(roundness, 0.0), 100.0);
    if (qFuzzyCompare(yRoundness, m_yRoundness)) {
        updateRoundnessHandles();
        return;
    }

    qDebug() << "Y" << yRoundness;
    prepareGeometryChange();
    m_yRoundness = yRoundness;
    update();
    updateRoundnessHandles();
}

void GraphicsRectItem::updateSizeHandles()
{
    blockItemNotification();
    qreal midX = m_rect.right()-m_rect.width()/2.0;
    qreal midY = m_rect.bottom()-m_rect.height()/2.0;
    m_idToHandle[TopLeft]->setPos(m_rect.topLeft());
    m_idToHandle[BottomRight]->setPos(m_rect.bottomRight());
    m_idToHandle[Bottom]->setPos(QPointF(midX, m_rect.bottom()));
    m_idToHandle[Left]->setPos(QPointF(m_rect.left(), midY));
    if (m_rect == m_rect.normalized()) {
        m_idToHandle[TopLeft]->setHandleRole(FDiagSizeHandleRole);
        m_idToHandle[BottomRight]->setHandleRole(FDiagSizeHandleRole);
    }
    else {
        m_idToHandle[TopLeft]->setHandleRole(BDiagSizeHandleRole);
        m_idToHandle[BottomRight]->setHandleRole(BDiagSizeHandleRole);
    }
    unblockItemNotification();
}

void GraphicsRectItem::updateRoundnessHandles()
{
    blockItemNotification();
    QRectF rect = m_rect.normalized();
    QPointF xPos(rect.right() - (m_xRoundness / 100.0) * (m_rect.width()  / 2.0), rect.top());
    m_idToHandle[XRoundness]->setPos(xPos);
    QPointF yPos(rect.right(), rect.top() + (m_yRoundness / 100.0) * (m_rect.height() / 2.0));
    m_idToHandle[YRoundness]->setPos(yPos);
    unblockItemNotification();
}

SchItem *GraphicsRectItem::clone()
{
    GraphicsRectItem *item = new GraphicsRectItem();
    SchItem::cloneTo(item);
    item->setRect(rect());
    item->setXRoundness(m_xRoundness);
    item->setYRoundness(m_yRoundness);
    return item;
}

void GraphicsRectItem::itemNotification(IGraphicsObservableItem *item)
{
    AbstractGraphicsHandle *handle = dynamic_cast<AbstractGraphicsHandle*>(item);
    Q_ASSERT(handle);

    int id = handle->handleId();
    QRectF rect = m_rect;
    QRectF normedRect = m_rect.normalized();
    switch (id) {
    case XRoundness:
        setXRoundness(100.0 * (normedRect.right() - handle->pos().x()) / (normedRect.width() / 2.0));
        return;
    case YRoundness:
        setYRoundness(100.0 * (handle->pos().y() - normedRect.top()) / (normedRect.height() / 2.0));
        return;
    case TopLeft:
        rect.setTopLeft(handle->pos());
        break;
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

bool GraphicsRectItem::fromJson(QString *errorString, const QJsonObject &jsonObject)
{
    if (!SchItem::fromJson(errorString, jsonObject))
        return false;
    if (!jsonObject.contains(J_POINTS)) {
        *errorString = "Rectangle item: missing points";
        return false;
    }
    QRectF rect;
    if (!Json::toRect(errorString, jsonObject.value(J_POINTS), rect))
        return false;
    setRect(rect);
    return true;
}

void GraphicsRectItem::toJson(QJsonObject &jsonObject) const
{
    SchItem::toJson(jsonObject);
    jsonObject.insert(J_POINTS, Json::fromRect(rect()));
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
    path.addRoundedRect(m_rect, m_xRoundness, m_yRoundness, Qt::RelativeSize);
    return shapeFromPath(path, pen());
}

void GraphicsRectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(pen());
    painter->setBrush(brush());
    painter->drawRoundedRect(m_rect, m_xRoundness, m_yRoundness, Qt::RelativeSize);
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


bool GraphicsRectItem::contains(const QPointF &point) const
{
    // FIXME: Needed to avoid slowing down the view: default impl of contains() call
    // shape() and QPainterPath implement the rounded corner with bezier curves,
    // which is CPU intensive. Relying on boundingRect() is super fast, but quite
    // inacurate, find and implement a good trade off b/w speed and precision
    return boundingRect().contains(point);
}
