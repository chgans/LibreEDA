#pragma once

#include <QGraphicsObject>
#include <QPointF>
#include <QRectF>
#include <QPainterPath>
#include <QVector>

#include "handle/handle.h"

#include "item/iitemobserver.h"

class QGraphicsItem;
class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;

namespace SymbolEditor
{

    class RegularHandle;
    class BezierHandle;

    // TODO: add properties
    // TODO: AbstractPath and AbstractShape (allow to morph between AbstractXYZ impl)
    // TODO: See qcad explodable concept

    class Item: public QGraphicsObject, public IItemObserver
    {
        Q_OBJECT

        Q_PROPERTY(QPen pen READ pen WRITE setPen NOTIFY penChanged)
        Q_PROPERTY(QBrush brush READ brush WRITE setBrush NOTIFY brushChanged)
        Q_PROPERTY(bool xMirrored READ isXMirrored WRITE setXMirrored NOTIFY xMirroredChanged)
        Q_PROPERTY(bool yMirrored READ isYMirrored WRITE setYMirrored NOTIFY yMirroredChanged)

    public:
        explicit Item(Item *parent = nullptr);
        virtual ~Item();

        virtual Item *clone() = 0;
        int handleCount() const;
        Handle *handleAt(int idx);

        QPen pen() const;
        QBrush brush() const;
        bool isXMirrored() const;
        bool isYMirrored() const;

        virtual QList<QPointF> hotSpots() const;
        virtual QList<QPointF> endPoints() const;
        virtual QList<QPointF> midPoints() const;
        virtual QList<QPointF> centerPoints() const;
        virtual QList<QPointF> nearestPoints(QPointF pos) const;

        virtual QList<QLineF> axes() const;

    public slots:
        void setPen(const QPen &pen);
        void setBrush(const QBrush &brush);
        void setXMirrored(bool mirrored);
        void setYMirrored(bool mirrored);

    signals:
        void penChanged(QPen pen);
        void brushChanged(QBrush brush);
        bool xMirroredChanged();
        bool yMirroredChanged();

    protected:
        QPen m_pen;
        QBrush m_brush;
        mutable QRectF m_boundingRect;

        QMap<Handle *, int> m_handleToId;
        QMap<int, Handle *> m_idToHandle;
        RegularHandle *addRegularHandle(int id, GraphicsHandleRole role, GraphicsHandleShape shape,
                                        const QPointF &pos = QPointF(0, 0));
        BezierHandle *addBezierHandle(int id, const QPointF &pos = QPointF(0, 0));
        void removeHandle(int id);
        void removeHandle(Handle *handle);
        void removeAllHandles();
        RegularHandle *regularHandleAt(int id) const;
        BezierHandle *bezierHandleAt(int id) const;

        void cloneTo(Item *dst);
        static QPainterPath shapeFromPath(const QPainterPath &path, const QPen &pen);

        bool m_isXMirrored;
        bool m_isYMirrored;
        void updateMirroringTransform();

    private:
        void addAbstractHandle(Handle *handle);

    };

}
