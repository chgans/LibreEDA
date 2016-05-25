#ifndef GRAPHICSPOLYGONITEM_H
#define GRAPHICSPOLYGONITEM_H

#include "item/item.h"

namespace SymbolEditor
{


    class PolygonItem : public Item
    {
        Q_OBJECT

        Q_PROPERTY(Qt::FillRule fillRule READ fillRule WRITE setFillRule NOTIFY fillRuleChanged)
        Q_PROPERTY(QPolygonF polygon READ polygon WRITE setPolygon NOTIFY polygonChanged)

    public:
        explicit PolygonItem(Item *parent = nullptr);
        ~PolygonItem();

        Qt::FillRule fillRule() const;
        QPolygonF polygon() const;

    public slots:
        void setFillRule(Qt::FillRule fillRule);
        void setPolygon(QPolygonF polygon);
        void addPoint(const QPointF &pos);
        void movePoint(int idx, const QPointF &pos);

    signals:
        void fillRuleChanged(Qt::FillRule fillRule);
        void polygonChanged(QPolygonF polygon);

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

#endif // GRAPHICSPOLYGONITEM_H
