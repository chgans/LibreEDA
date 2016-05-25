#pragma once

#include "item/item.h"

#include <QPointF>
#include <QList>

class QGraphicsPathItem;

namespace SymbolEditor
{

    class WireItem : public Item
    {
        Q_OBJECT

        Q_PROPERTY(QList<QPointF> points READ points WRITE setPoints NOTIFY pointsChanged)

    public:
        explicit WireItem(Item *parent = nullptr);
        ~WireItem();

        QList<QPointF> points() const;
        void addPoint(const QPointF &pos);
        void movePoint(int idx, const QPointF &pos);

    public slots:
        void setPoints(QList<QPointF> points);

    signals:
        void pointsChanged();

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
