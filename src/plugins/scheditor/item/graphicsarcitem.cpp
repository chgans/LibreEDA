#include "item/graphicsarcitem.h"

#include <qmath.h>
#include <QStyleOptionGraphicsItem>

GraphicsArcItem::GraphicsArcItem(SchItem *parent):
    SchItem(parent),
    m_xRadius(0.0), m_yRadius(0.0),
    m_startAngle(0.0), m_spanAngle(360.0 * 16)
{
    addHandles();
}

GraphicsArcItem::GraphicsArcItem(qreal xRadius, qreal yRadius, SchItem *parent):
    SchItem(parent),
    m_xRadius(xRadius), m_yRadius(yRadius),
    m_startAngle(0.0), m_spanAngle(360.0 * 16)
{
    addHandles();
}

GraphicsArcItem::GraphicsArcItem(qreal xRadius, qreal yRadius,
                                 int startAngle, int spanAngle, SchItem *parent):
    SchItem(parent),
    m_xRadius(xRadius), m_yRadius(yRadius),
    m_startAngle(startAngle), m_spanAngle(spanAngle)
{
    addHandles();
}

GraphicsArcItem::~GraphicsArcItem()
{

}

qreal GraphicsArcItem::xRadius() const
{
    return m_xRadius;
}

qreal GraphicsArcItem::yRadius() const
{
    return m_yRadius;
}

int GraphicsArcItem::startAngle() const
{
    return m_startAngle;
}

int GraphicsArcItem::spanAngle() const
{
    return m_spanAngle;
}

void GraphicsArcItem::setXRadius(qreal r)
{
    qreal radius = qAbs(r);
    if (qFuzzyCompare(radius, m_xRadius))
    {
        return;
    }
    prepareGeometryChange();
    m_boundingRect = QRectF();
    m_xRadius = radius;
    update();
    updateHandles();
}

void GraphicsArcItem::setYRadius(qreal r)
{
    qreal radius = qAbs(r);
    if (qFuzzyCompare(radius, m_yRadius))
    {
        return;
    }
    prepareGeometryChange();
    m_boundingRect = QRectF();
    m_yRadius = radius;
    update();
    updateHandles();
}

void GraphicsArcItem::setStartAngle(int a)
{
    qreal angle = fmod(a, 360 * 16);
    if (angle == m_startAngle)
    {
        return;
    }
    prepareGeometryChange();
    m_boundingRect = QRectF();
    m_startAngle = angle;
    update();
    updateHandles();
}

void GraphicsArcItem::setSpanAngle(int a)
{
    qreal angle = fmod(a, 360 * 16);
    if (angle == m_spanAngle)
    {
        return;
    }
    prepareGeometryChange();
    m_boundingRect = QRectF();
    m_spanAngle = angle;
    update();
    updateHandles();
}

void GraphicsArcItem::addHandles()
{
    addRegularHandle(XRadiusHandle, MoveHandleRole, DiamondedHandleShape);
    addRegularHandle(YRadiusHandle, MoveHandleRole, DiamondedHandleShape);
    addRegularHandle(StartAngleHandle, MoveHandleRole, CircularHandleShape);
    addRegularHandle(SpanAngleHandle, MoveHandleRole, CircularHandleShape);
}

void GraphicsArcItem::updateHandles()
{
    blockItemNotification();
    handleAt(XRadiusHandle)->setPos(QPointF(m_xRadius, 0.0));
    handleAt(YRadiusHandle)->setPos(QPointF(0.0, m_yRadius));
    handleAt(StartAngleHandle)->setPos(pointAt(m_startAngle));
    handleAt(SpanAngleHandle)->setPos(pointAt(m_startAngle + m_spanAngle));
    unblockItemNotification();
}

QPointF GraphicsArcItem::pointAt(int angle) const
{
    qreal theta = qDegreesToRadians(angle / 16.0);
    return QPointF(m_xRadius * qCos(theta), -m_yRadius * qSin(theta));
}

qreal GraphicsArcItem::angleAt(const QPointF &pos) const
{
    QLineF vector(QPointF(0, 0), QPointF(pos.x() / xRadius(), pos.y() / m_yRadius));
    return int(16 * vector.angle());
}

QRectF GraphicsArcItem::rect() const
{
    return QRectF(-m_xRadius, -m_yRadius, m_xRadius * 2.0, m_yRadius * 2.0);
}

QRectF GraphicsArcItem::boundingRect() const
{
    if (m_boundingRect.isNull())
    {
        qreal pw = pen().style() == Qt::NoPen ? qreal(0) : pen().widthF();
        if (pw == 0.0 && m_spanAngle == 360 * 16)
        {
            m_boundingRect = rect();
        }
        else
        {
            m_boundingRect = shape().controlPointRect();
        }
    }
    return m_boundingRect;
}

QPainterPath GraphicsArcItem::shape() const
{
    QPainterPath path;
    if (m_spanAngle != 360.0 * 16)
    {
        path.arcTo(rect(), m_startAngle / 16, m_spanAngle / 16);
    }
    else
    {
        path.addEllipse(rect());
    }

    return shapeFromPath(path, pen());
}

void GraphicsArcItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                            QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(pen());
    painter->setBrush(brush());
    if ((m_spanAngle != 0) && (qAbs(m_spanAngle) % (360 * 16) == 0))
    {
        painter->drawEllipse(rect());
    }
    else
    {
        painter->drawPie(rect(), m_startAngle, m_spanAngle);
    }

    if (option->state & QStyle::State_Selected)
    {
        painter->setBrush(Qt::NoBrush);
        painter->setPen(QPen(QBrush(Qt::red), 0, Qt::DashLine));
        painter->drawRect(rect());
    }
}

QVariant GraphicsArcItem::itemChange(QGraphicsItem::GraphicsItemChange change,
                                     const QVariant &value)
{
    if (change == QGraphicsItem::ItemSelectedHasChanged)
    {
        for (int i = 0; i < handleCount(); i++)
        {
            handleAt(i)->setVisible(isSelected());
        }
    }
    return value;
}

void GraphicsArcItem::itemNotification(IGraphicsObservableItem *item)
{
    AbstractGraphicsHandle *handle = static_cast<AbstractGraphicsHandle *>(item);
    int angle = angleAt(handle->pos());
    switch (handle->handleId())
    {
        case XRadiusHandle:
            setXRadius(handle->x());
            break;
        case YRadiusHandle:
            setYRadius(handle->y());
            break;
        case StartAngleHandle:
            setStartAngle(angle);
            break;
        case SpanAngleHandle:
            setSpanAngle(angle + 360 * 16 - m_startAngle);
            break;
        default:
            Q_ASSERT(false);
            break;
    }
}

SchItem *GraphicsArcItem::clone()
{
    GraphicsArcItem *item = new GraphicsArcItem();
    item->setXRadius(m_xRadius);
    item->setYRadius(m_yRadius);
    item->setStartAngle(m_startAngle);
    item->setSpanAngle(m_spanAngle);
    SchItem::cloneTo(item);
    return item;
}

QList<QPointF> GraphicsArcItem::endPoints() const
{
    return QList<QPointF>() << pointAt(m_startAngle) << pointAt(m_startAngle + m_spanAngle);
}

QList<QPointF> GraphicsArcItem::midPoints() const
{
    return QList<QPointF>() << pointAt(m_startAngle + m_spanAngle / 2.0);
}

QList<QPointF> GraphicsArcItem::centerPoints() const
{
    return QList<QPointF>() << QPointF(0, 0);
}

QList<QPointF> GraphicsArcItem::nearestPoints(QPointF pos) const
{
    int theta = angleAt(pos);
    if (theta > 180 * 16)
    {
        theta -= 360 * 16;
    }

    int startAngle = m_startAngle;
    if (startAngle > 180 * 16)
    {
        startAngle -= 360 * 16;
    }

    int diffStart = (theta - startAngle);
    if (diffStart < 0)
    {
        diffStart += 360 * 16;
    }

    if (diffStart >= 0 && diffStart <= m_spanAngle)
    {
        return QList<QPointF>() << pointAt(theta);
    }
    else
    {
        return QList<QPointF>();
    }
}
