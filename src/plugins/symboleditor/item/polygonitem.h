#pragma once

#include "item/item.h"

namespace SymbolEditor
{


    class PolygonItem : public Item
    {
    public:
        explicit PolygonItem(Item *parent = nullptr);
        ~PolygonItem();

        Qt::FillRule fillRule() const;
        void setFillRule(Qt::FillRule fillRule);
        QPolygonF polygon() const;
        void setPolygon(QPolygonF polygon);

        void addPoint(const QPointF &pos);
        void movePoint(int idx, const QPointF &pos);

    private:
        Qt::FillRule m_fillRule;
        QPolygonF m_polygon;
        void handleToPolygon();
        void polygonToHandle();

        // QGraphicsItem interface
    public:
        virtual QRectF boundingRect() const;
        virtual QPainterPath shape() const;
        virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

        // SchItem interface
    public:
        virtual Item *clone();
        void setProperty(quint64 id, const QVariant &value);

        //    //virtual QList<QPointF> hotSpots() const;
        //    virtual QList<QPointF> endPoints() const;
        //    virtual QList<QPointF> midPoints() const;
        //    virtual QList<QPointF> centerPoints() const;
        //    virtual QList<QPointF> nearestPoints(QPointF pos) const;

        // QGraphicsItem interface
    protected:
        virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);

        // IGraphicsItemObserver interface
    public:
        virtual void itemNotification(IObservableItem *item);
    };

}
