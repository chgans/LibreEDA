#ifndef ABSTRACTGRAPHICSHANDLE_H
#define ABSTRACTGRAPHICSHANDLE_H

#include <QGraphicsPathItem>
#include "graphicseditor/igraphicsobservableitem.h"

#include <QPainter>
#include <QPointer>
#include <QCursor>

class GraphicsObject;
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
    AbstractGraphicsHandle(GraphicsObject *parent);
    virtual ~AbstractGraphicsHandle();

    QCursor handleCursor() const;

    virtual void setHandleId(int id);
    int handleId() const;
    void setHandleRole(GraphicsHandleRole role);
    GraphicsHandleRole handleRole() const;
    void setHandleShape(GraphicsHandleShape shape);
    GraphicsHandleShape handleShape() const;
    GraphicsObject *parentGraphicsObject() const;
    virtual void setParentGraphicsObject(GraphicsObject *parent);

private:
    static QCursor roleToCursor(GraphicsHandleRole role);
    int m_id;
    GraphicsHandleRole m_role;
    GraphicsHandleShape m_handleShape;
    GraphicsHandleBehaviour m_behaviour;
    GraphicsObject *m_parent;
    void updateShape();

    // QGraphicsItem interface
protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
};

#endif // ABSTRACTGRAPHICSHANDLE_H
