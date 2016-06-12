#pragma once

#include "item/item.h"

#include <QPen>
#include <QLineF>

namespace SymbolEditor
{

    class PolylineItem : public Item
    {
    public:
        enum HandleId
        {
            P1Handle = 0,
            P2Handle,
            NbHandles
        };
        explicit PolylineItem(Item *parent = nullptr);

        QLineF line() const;
        void setLine(const QLineF &line);

    private:
        QLineF m_line;

        // SchItem interface
    public:
        Item *clone();
        void setProperty(quint64 id, const QVariant &value);

        //virtual QList<QPointF> hotSpots() const;
        virtual QList<QPointF> endPoints() const;
        virtual QList<QPointF> midPoints() const;
        virtual QList<QPointF> centerPoints() const;
        virtual QList<QPointF> nearestPoints(QPointF pos) const;

        // IGraphicsItemObserver
    public:
        void itemNotification(IObservableItem *item);

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
