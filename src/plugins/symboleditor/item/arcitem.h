#pragma once

#include "item/item.h"

// TODO: Pie vs chord vs whole shape
// TODO: keep ratio (ellipse vs circle)

namespace SymbolEditor
{

    class ArcItem : public Item
    {
        Q_OBJECT

        Q_PROPERTY(qreal xRadius READ xRadius WRITE setXRadius)
        Q_PROPERTY(qreal yRadius READ yRadius WRITE setYRadius)
        Q_PROPERTY(qreal startAngle READ startAngle WRITE setStartAngle)
        Q_PROPERTY(qreal spanAngle READ spanAngle WRITE setSpanAngle)

    public:
        enum HandleId
        {
            XRadiusHandle = 0,
            YRadiusHandle,
            StartAngleHandle,
            SpanAngleHandle
        };

        explicit ArcItem(Item *parent = nullptr);
        explicit ArcItem(qreal xRadius, qreal yRadius, Item *parent = nullptr);
        explicit ArcItem(qreal xRadius, qreal yRadius, int startAngle, int spanAngle,
                         Item *parent = nullptr);
        ~ArcItem();

        qreal xRadius() const;
        qreal yRadius() const;
        int startAngle() const;
        int spanAngle() const;

    public slots:
        void setXRadius(qreal radius);
        void setYRadius(qreal radius);
        void setStartAngle(int angle);
        void setSpanAngle(int angle);

    private:
        void addHandles();
        void updateHandles();
        QPointF pointAt(int angle) const;
        qreal angleAt(const QPointF &pos) const;
        QRectF rect() const;
        //inline qreal spanAngle() const;
        qreal m_xRadius;
        qreal m_yRadius;
        int m_startAngle;
        int m_spanAngle;
        mutable QRectF m_boundingRect;

        // QGraphicsItem interface
    public:
        virtual QRectF boundingRect() const;
        virtual QPainterPath shape() const;
        virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

        // QGraphicsItem interface
    protected:
        virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);

        // IGraphicsItemObserver interface
    public:
        virtual void itemNotification(IObservableItem *item);

        // SchItem interface
    public:
        virtual Item *clone();

        //virtual QList<QPointF> hotSpots() const;
        virtual QList<QPointF> endPoints() const;
        virtual QList<QPointF> midPoints() const;
        virtual QList<QPointF> centerPoints() const;
        virtual QList<QPointF> nearestPoints(QPointF pos) const;
    };

}
