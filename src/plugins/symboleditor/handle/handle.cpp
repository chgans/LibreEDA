#include "item/item.h"
#include "handle/handle.h"
#include "view/palette.h"

#include <QStyleOptionGraphicsItem>
#include <QPainterPathStroker>
#include <QPainter>

using namespace SymbolEditor;

Handle::Handle(Item *parent):
    QGraphicsPathItem(parent),
    IObservableItem(),
    m_role(MoveHandleRole),
    m_handleShape(CircularHandleShape),
    m_behaviour(NormalHandleBehaviour),
    m_parent(parent) // hackish: Because BezierHandle act as a proxy
{
    // FIXME: From palette
    setPen(QPen(QColor("#839496"), 0)); // Primary content
    setBrush(QColor("#073642")); // BG

    //    setPen(QPen(gpalette::Content2, 0));
    //    setBrush(QBrush(gpalette::Content4));

    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);

    updateShape();

    setVisible(false);
}

Handle::~Handle()
{

}

QCursor Handle::handleCursor() const
{
    return roleToCursor(m_role);
}

void Handle::setHandleId(int id)
{
    m_id = id;
}

int Handle::handleId() const
{
    return m_id;
}

void Handle::setHandleRole(GraphicsHandleRole role)
{
    if (m_role == role)
    {
        return;
    }

    prepareGeometryChange();
    m_role = role;
    update();
}

GraphicsHandleRole Handle::handleRole() const
{
    return m_role;
}

void Handle::setHandleShape(GraphicsHandleShape shape)
{
    if (m_handleShape == shape)
    {
        return;
    }

    m_handleShape = shape;

    updateShape();
}

GraphicsHandleShape Handle::handleShape() const
{
    return m_handleShape;
}

Item *Handle::parentGraphicsObject() const
{
    return m_parent;
}

void Handle::setParentGraphicsObject(Item *parent)
{
    m_parent = parent;
    // TBD: we could auto-add the observer
}

QCursor Handle::roleToCursor(GraphicsHandleRole role)
{
    switch (role)
    {
        case MoveHandleRole:
            return QCursor(Qt::PointingHandCursor);
        case VSizeHandleRole:
            return QCursor(Qt::SizeVerCursor);
        case HSizeHandleRole:
            return QCursor(Qt::SizeHorCursor);
        case BDiagSizeHandleRole:
            return QCursor(Qt::SizeBDiagCursor);
        case FDiagSizeHandleRole:
            return QCursor(Qt::SizeFDiagCursor);
        case RotateHandleRole:
            return QCursor(); // TBD
        case ShearHandleRole:
            return QCursor();  // TBD
        case MarkHandleRole:
            return QCursor();   // TBD
        default:
            return QCursor();
    }
}

void Handle::updateShape()
{
    int radius = 5;
    QPainterPath path;
    switch (m_handleShape)
    {
        case CircularHandleShape:
            radius *= 0.97;
            path.addEllipse(QPointF(0, 0), radius, radius);
            break;
        case SquaredHandleShape:
            radius *= 0.8;
            path.addRect(-radius, -radius, 2 * radius, 2 * radius);
            break;
        case DiamondedHandleShape:
        {
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


QVariant Handle::itemChange(QGraphicsItem::GraphicsItemChange change,
                                            const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionHasChanged)
    {
        notifyObservers();
    }
    return value;
}
