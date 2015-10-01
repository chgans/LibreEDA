#ifndef ABSTRACTGRAPHICSHANDLE_H
#define ABSTRACTGRAPHICSHANDLE_H

#include <QGraphicsPathItem>
#include "item/igraphicsobservableitem.h"

#include <QPainter>
#include <QPointer>
#include <QCursor>

class SchItem;
class QStyleOptionGraphicsItem;
class QPainter;
class QWidget;

enum GraphicsHandleRole {
    MoveHandleRole = 0,
    VSizeHandleRole,
    HSizeHandleRole,
    BDiagSizeHandleRole,
    FDiagSizeHandleRole,
    RotateHandleRole,
    ShearHandleRole,
    MarkHandleRole
};

enum GraphicsHandleShape {
    CircularHandleShape = 0,
    SquaredHandleShape,
    DiamondedHandleShape
};

enum GraphicsHandleBehaviour {
    NormalHandleBehaviour = 0,
    CornerHandleBehaviour,
    SmoothHandleBehaviour,
    SymetricHandleBehaviour,
    AutoSmoothHandleBehaviour
};

class AbstractGraphicsHandle: public QGraphicsPathItem, public IGraphicsObservableItem
{
public:
    AbstractGraphicsHandle(SchItem *parent);
    virtual ~AbstractGraphicsHandle();

    QCursor handleCursor() const;

    virtual void setHandleId(int id);
    int handleId() const;
    void setHandleRole(GraphicsHandleRole role);
    GraphicsHandleRole handleRole() const;
    void setHandleShape(GraphicsHandleShape shape);
    GraphicsHandleShape handleShape() const;
    SchItem *parentGraphicsObject() const;
    virtual void setParentGraphicsObject(SchItem *parent);

private:
    static QCursor roleToCursor(GraphicsHandleRole role);
    int m_id;
    GraphicsHandleRole m_role;
    GraphicsHandleShape m_handleShape;
    GraphicsHandleBehaviour m_behaviour;
    SchItem *m_parent;
    void updateShape();

    // QGraphicsItem interface
protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
};

#endif // ABSTRACTGRAPHICSHANDLE_H
