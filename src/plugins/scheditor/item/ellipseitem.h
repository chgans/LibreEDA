#ifndef GRAPHICSELLIPSEITEM_H
#define GRAPHICSELLIPSEITEM_H

#include "item/item.h"

class EllipseItem : public Item
{
    Q_OBJECT

    Q_PROPERTY(qreal xRadius READ xRadius WRITE setXRadius NOTIFY xRadiusChanged)
    Q_PROPERTY(qreal yRadius READ yRadius WRITE setYRadius NOTIFY yRadiusChanged)

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
    qreal yRadius() const;

public slots:
    void setXRadius(qreal xRadius);
    void setYRadius(qreal yRadius);

signals:
    void xRadiusChanged(qreal xRadius);
    void yRadiusChanged(qreal yRadius);

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

#endif // GRAPHICSELLIPSEITEM_H
