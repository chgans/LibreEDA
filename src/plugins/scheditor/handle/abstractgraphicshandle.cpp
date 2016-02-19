#include "item/schitem.h"
#include "handle/abstractgraphicshandle.h"
#include "palette.h"

#include <QStyleOptionGraphicsItem>
#include <QPainterPathStroker>
#include <QPainter>

AbstractGraphicsHandle::AbstractGraphicsHandle(SchItem *parent):
    QGraphicsPathItem(parent),
    IGraphicsObservableItem(),
    m_role(MoveHandleRole),
    m_handleShape(CircularHandleShape),
    m_behaviour(NormalHandleBehaviour),
    m_parent(parent) // hackish: Because BezierHandle act as a proxy
{
    // FIXME: From palette
    setPen(QPen(QBrush(Qt::red), 0));
    setBrush(QBrush(Qt::white));

//    setPen(QPen(gpalette::Content2, 0));
//    setBrush(QBrush(gpalette::Content4));

    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);

    updateShape();    

    setVisible(false);
}

AbstractGraphicsHandle::~AbstractGraphicsHandle()
{

}

QCursor AbstractGraphicsHandle::handleCursor() const
{
    return roleToCursor(m_role);
}

void AbstractGraphicsHandle::setHandleId(int id)
{
    m_id = id;
}

int AbstractGraphicsHandle::handleId() const
{
    return m_id;
}

void AbstractGraphicsHandle::setHandleRole(GraphicsHandleRole role)
{
    if (m_role == role)
        return;

    prepareGeometryChange();
    m_role = role;
    update();
}

GraphicsHandleRole AbstractGraphicsHandle::handleRole() const
{
    return m_role;
}

void AbstractGraphicsHandle::setHandleShape(GraphicsHandleShape shape)
{
    if (m_handleShape == shape)
        return;

    m_handleShape = shape;

    updateShape();
}

GraphicsHandleShape AbstractGraphicsHandle::handleShape() const
{
    return m_handleShape;
}

SchItem *AbstractGraphicsHandle::parentGraphicsObject() const
{
    return m_parent;
}

void AbstractGraphicsHandle::setParentGraphicsObject(SchItem *parent)
{
    m_parent = parent;
    // TBD: we could auto-add the observer
}

QCursor AbstractGraphicsHandle::roleToCursor(GraphicsHandleRole role)
{
    switch (role)
    {
    case MoveHandleRole: return QCursor(Qt::PointingHandCursor);
    case VSizeHandleRole: return QCursor(Qt::SizeVerCursor);
    case HSizeHandleRole: return QCursor(Qt::SizeHorCursor);
    case BDiagSizeHandleRole: return QCursor(Qt::SizeBDiagCursor);
    case FDiagSizeHandleRole: return QCursor(Qt::SizeFDiagCursor);
    case RotateHandleRole: return QCursor(); // TBD
    case ShearHandleRole: return QCursor();  // TBD
    case MarkHandleRole: return QCursor();   // TBD
    default: return QCursor();
    }
}

void AbstractGraphicsHandle::updateShape()
{
    int radius = 5;
    QPainterPath path;
    switch (m_handleShape) {
    case CircularHandleShape:
        radius *= 0.97;
        path.addEllipse(QPointF(0, 0), radius, radius);
        break;
    case SquaredHandleShape:
        radius *= 0.8;
        path.addRect(-radius, -radius, 2*radius, 2*radius);
        break;
    case DiamondedHandleShape: {
        QVector<QPointF> points;
        points << QPointF(0, -radius) << QPointF(radius, 0)
               << QPointF(0, radius) << QPointF(-radius, 0);
        path.addPolygon(QPolygonF(points));
        path.closeSubpath();
        break;
    }
    default:
        // Not reached
        break;
    }
    setPath(path);
}


QVariant AbstractGraphicsHandle::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionHasChanged) {
        notifyObservers();
    }
    return value;
}
