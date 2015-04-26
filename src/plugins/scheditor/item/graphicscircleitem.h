#ifndef GRAPHICSCIRCLEITEM_H
#define GRAPHICSCIRCLEITEM_H

#include "schitem.h"

class GraphicsCircleItem : public SchItem
{
    Q_OBJECT

    Q_PROPERTY(qreal radius READ radius WRITE setRadius NOTIFY radiusChanged)

public:
    enum HandleId {
        RadiusHandle = 0,
        NbHandles
    };

    explicit GraphicsCircleItem(SchItem *parent = 0);
    ~GraphicsCircleItem();

    qreal radius() const;

public slots:
    void setRadius(qreal length);

signals:
    void radiusChanged();

private:
    qreal m_radius;

    // GraphicsObject interface
public:
    virtual SchItem *clone();
    void itemNotification(IGraphicsObservableItem *item);

    // QGraphicsItem interface
public:
    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    // QGraphicsItem interface
protected:
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);
};

#endif // GRAPHICSCIRCLEITEM_H
