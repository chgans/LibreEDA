#pragma once

#include "item/item.h"

#include <QVector>
#include <QMap>

#include <QPen>
#include <QBrush>

namespace SymbolEditor
{

    // TODO: AbstractGraphicsShape
    // TODO: add properties
    class RectangleItem : public Item
    {
    public:
        enum HandleId
        {
            TopLeft = 0,
            BottomRight,
            Bottom,
            Left,
            XRoundness,
            YRoundness,
            NbHandles
        };

        explicit RectangleItem(Item *parent = nullptr);
        ~RectangleItem();

        QRectF rect() const;
        void setRect(const QRectF &rect);

        void setRoundness(qreal xRoundness, qreal yRoundness);
        qreal xRoundness() const;
        void setXRoundness(qreal roundness);
        qreal yRoundness() const;
        void setYRoundness(qreal roundness);

    private:
        QRectF m_rect;
        qreal m_xRoundness;
        qreal m_yRoundness;
        void updateSizeHandles();
        void updateRoundnessHandles();

        // ScheItem interface
    public:
        virtual Item *clone();
        void itemNotification(IObservableItem *item);
        virtual void setProperty(quint64 id, const QVariant &value);

        //virtual QList<QPointF> hotSpots() const;
        virtual QList<QPointF> endPoints() const;
        virtual QList<QPointF> midPoints() const;
        virtual QList<QPointF> centerPoints() const;
        virtual QList<QPointF> nearestPoints(QPointF pos) const;

        // QGraphicsItem interface
    public:
        virtual QRectF boundingRect() const;
        virtual QPainterPath shape() const;
        virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

        // QGraphicsItem interface
    protected:
        virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);

        // QGraphicsItem interface
    public:
        virtual bool contains(const QPointF &point) const;
    };

}
