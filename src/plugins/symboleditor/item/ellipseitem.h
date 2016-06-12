#pragma once

#include "item/item.h"

namespace SymbolEditor
{

    class EllipseItem : public Item
    {
    public:
        enum HandleId
        {
            XRadiusHandle = 0,
            YRadiusHandle,
            NbHandles
        };

        explicit EllipseItem(Item *parent = nullptr);
        ~EllipseItem();

        qreal xRadius() const;
        void setXRadius(qreal xRadius);
        qreal yRadius() const;
        void setYRadius(qreal yRadius);

    private:
        QPointF pointAt(int angle) const;
        qreal angleAt(const QPointF &pos) const;
        qreal m_xRadius;
        qreal m_yRadius;

        // QGraphicsItem interface
    public:
        virtual QRectF boundingRect() const;
        virtual QPainterPath shape() const;
        virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

        // SchItem interface
    public:
        virtual Item *clone();
        virtual void setProperty(quint64 id, const QVariant &value);

        //virtual QList<QPointF> hotSpots() const;
        virtual QList<QPointF> endPoints() const;
        virtual QList<QPointF> midPoints() const;
        virtual QList<QPointF> centerPoints() const;
        virtual QList<QPointF> nearestPoints(QPointF pos) const;

        // QGraphicsItem interface
    protected:
        virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);

        // IGraphicsItemObserver interface
    public:
        virtual void itemNotification(IObservableItem *item);
    };

}
