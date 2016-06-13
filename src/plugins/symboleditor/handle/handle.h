#pragma once

#include <QGraphicsPathItem>
#include "item/iobservableitem.h"
#include "view/palette.h"

#include <QPainter>
#include <QPointer>
#include <QCursor>
class QStyleOptionGraphicsItem;
class QPainter;
class QWidget;

namespace SymbolEditor
{

    class Item;

    enum GraphicsHandleRole
    {
        MoveHandleRole = 0,
        VSizeHandleRole,
        HSizeHandleRole,
        BDiagSizeHandleRole,
        FDiagSizeHandleRole,
        RotateHandleRole,
        ShearHandleRole,
        MarkHandleRole
    };

    enum GraphicsHandleShape
    {
        CircularHandleShape = 0,
        SquaredHandleShape,
        DiamondedHandleShape
    };

    enum GraphicsHandleBehaviour
    {
        NormalHandleBehaviour = 0,
        CornerHandleBehaviour,
        SmoothHandleBehaviour,
        SymetricHandleBehaviour,
        AutoSmoothHandleBehaviour
    };

    class Handle: public QGraphicsPathItem, public IObservableItem
    {
    public:
        explicit Handle(Item *parent = nullptr);
        virtual ~Handle();

        QCursor handleCursor() const;

        virtual void setHandleId(int id);
        int handleId() const;
        void setHandleRole(GraphicsHandleRole role);
        GraphicsHandleRole handleRole() const;
        void setHandleShape(GraphicsHandleShape shape);
        GraphicsHandleShape handleShape() const;
        Item *parentGraphicsObject() const;
        virtual void setParentGraphicsObject(Item *parent);

        void setPalette(Palette palette);
        Palette palette() const;

    private:
        Palette m_palette;
        static QCursor roleToCursor(GraphicsHandleRole role);
        int m_id;
        GraphicsHandleRole m_role;
        GraphicsHandleShape m_handleShape;
        GraphicsHandleBehaviour m_behaviour;
        Item *m_parent;
        void updateShape();

        // QGraphicsItem interface
    protected:
        QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    };

}
