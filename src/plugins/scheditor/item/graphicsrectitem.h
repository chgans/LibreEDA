#ifndef GRAPHICSRECTITEM_H
#define GRAPHICSRECTITEM_H

#include "item/schitem.h"

#include <QVector>
#include <QMap>

#include <QPen>
#include <QBrush>

// TODO: AbstractGraphicsShape
// TODO: add properties
class GraphicsRectItem : public SchItem
{
    Q_OBJECT

    Q_PROPERTY(qreal xRoundness READ xRoundness WRITE setXRoundness)
    Q_PROPERTY(qreal yRoundness READ yRoundness WRITE setYRoundness)
    Q_PROPERTY(QRectF rect READ rect WRITE setRect)

public:
    enum HandleId {
        TopLeft = 0,
        BottomRight,
        Bottom,
        Left,
        XRoundness,
        YRoundness,
        NbHandles
    };

    explicit GraphicsRectItem(SchItem *parent = nullptr);
    ~GraphicsRectItem();

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
    virtual SchItem *clone();
    void itemNotification(IGraphicsObservableItem *item);

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

#endif // GRAPHICSRECTITEM_H
