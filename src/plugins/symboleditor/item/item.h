#pragma once

#include "handle/handle.h"
#include "item/iitemobserver.h"
#include "view/palette.h"
#include "xdl/symbol.h"

#include <QGraphicsItem>
#include <QPointF>
#include <QRectF>
#include <QPainterPath>
#include <QVector>


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

    class Item: public QGraphicsItem, public IItemObserver
    {

    public:
        typedef xdl::symbol::LineStyle LineStyle;
        typedef xdl::symbol::LineWidth LineWidth;
        typedef xdl::symbol::FillStyle FillStyle;
        typedef xdl::symbol::Color Color;

        explicit Item(Item *parent = nullptr);
        virtual ~Item();

        virtual Item *clone() = 0;
        int handleCount() const;
        Handle *handleAt(int idx);

        void setLineStyle(LineStyle style);
        LineStyle lineStyle() const;
        void setLineWidth(LineWidth width);
        LineWidth lineWidth() const;
        void setLineColor(Color color);
        Color lineColor() const;
        void setFillStyle(FillStyle style);
        FillStyle fillStyle() const;
        void setFillColor(Color color);
        Color fillColor() const;
        bool isXMirrored() const;
        void setXMirrored(bool mirrored);
        bool isYMirrored() const;
        void setYMirrored(bool mirrored);

        virtual QList<QPointF> hotSpots() const;
        virtual QList<QPointF> endPoints() const;
        virtual QList<QPointF> midPoints() const;
        virtual QList<QPointF> centerPoints() const;
        virtual QList<QPointF> nearestPoints(QPointF pos) const;

        virtual QList<QLineF> axes() const;

        virtual void setProperty(quint64 id, const QVariant &value);

        void setPalette(Palette palette);
        Palette palette() const;

    protected:
        Palette m_palette;

        LineStyle m_lineStyle;
        LineWidth m_lineWidth;
        Color m_lineColor;
        FillStyle m_fillStyle;
        Color m_fillColor;

        QPen pen() const;
        QBrush brush() const;

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
