#pragma once

#include "item/item.h"

#include <QPointF>
#include <QList>

class QGraphicsPathItem;

namespace SymbolEditor
{

    class WireItem : public Item
    {
    public:
        explicit WireItem(Item *parent = nullptr);
        ~WireItem();

        QList<QPointF> points() const;
        void setPoints(QList<QPointF> points);

        void addPoint(const QPointF &pos);
        void movePoint(int idx, const QPointF &pos);

    private:
        QPainterPath m_path;
        //void handleToPath();
        //void pathToHandle();

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

        // IGraphicsItemObserver interface
    public:
        virtual void itemNotification(IObservableItem *item);

        // QGraphicsItem interface
    protected:
        virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    };

}
