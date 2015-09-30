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

    // SchItem interface
public:
    virtual SchItem *clone();
    void itemNotification(IGraphicsObservableItem *item);
    virtual bool fromJson(QString *errorString, const QJsonObject &jsonObject);
    virtual void toJson(QJsonObject &jsonObject) const;

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
};

#endif // GRAPHICSCIRCLEITEM_H
