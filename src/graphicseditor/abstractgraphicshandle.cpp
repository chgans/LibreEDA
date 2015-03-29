#include "graphicsobject.h"
#include "abstractgraphicshandle.h"

#include <QStyleOptionGraphicsItem>
#include <QPainterPathStroker>
#include <QPainter>

#include <QDebug>

AbstractGraphicsHandle::AbstractGraphicsHandle(GraphicsObject *parent):
    QGraphicsPathItem(parent),
    IGraphicsObservableItem(),
    m_role(MoveHandleRole),
    m_handleShape(CircularHandleShape),
    m_behaviour(NormalHandleBehaviour),
    m_parent(parent) // hackish: Because BezierHandle act as a proxy
{
    setPen(QPen(QBrush(Qt::red), 0));
    setBrush(QBrush(Qt::white));

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

GraphicsObject *AbstractGraphicsHandle::parentGraphicsObject() const
{
    return m_parent;
}

void AbstractGraphicsHandle::setParentGraphicsObject(GraphicsObject *parent)
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
    static const int radius = 5;
    QPainterPath path;
    switch (m_handleShape) {
    case CircularHandleShape:
        path.addEllipse(QPointF(0, 0), radius, radius);
        break;
    case SquaredHandleShape:
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
