#pragma once

#include "item/item.h"

namespace SymbolEditor
{

    class CircleItem : public Item
    {
    public:
        enum HandleId
        {
            RadiusHandle = 0,
            NbHandles
        };

        explicit CircleItem(Item *parent = nullptr);
        ~CircleItem();

        qreal radius() const;
        void setRadius(qreal length);

    private:
        qreal m_radius;

        // SchItem interface
    public:
        virtual Item *clone();
        void itemNotification(IObservableItem *item);
        void setProperty(quint64 id, const QVariant &value);

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
    };

}
