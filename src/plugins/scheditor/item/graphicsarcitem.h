#ifndef GRAPHICSARCITEM_H
#define GRAPHICSARCITEM_H

#include "schitem.h"

class GraphicsArcItem : public SchItem
{

public:
    enum HandleId {
        RectHandle = 0,
        StartAngleHandle,
        SpanAngleHandle
    };

    GraphicsArcItem(SchItem *parent = 0);
    GraphicsArcItem(const QRectF &rect, SchItem *parent = 0);
    GraphicsArcItem(qreal x, qreal y, qreal w, qreal h, SchItem *parent = 0);
    ~GraphicsArcItem();

    QRectF rect() const;
    int startAngle() const;
    int spanAngle() const;

public slots:
    void setRect(const QRectF &rect);
    inline void setRect(qreal x, qreal y, qreal w, qreal h)
    { setRect(QRectF(x, y, w, h)); }
    void setStartAngle(int angle);
    void setSpanAngle(int angle);

private:
    void addHandles();
    void updateHandles();
    QPointF pointAt(int angle);
    int angleAt(const QPointF &pos);
    QRectF m_rect;
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
    virtual void itemNotification(IGraphicsObservableItem *item);

    // SchItem interface
public:
    virtual SchItem *clone();
    virtual bool fromJson(QString *errorString, const QJsonObject &jsonObject);
    virtual void toJson(QJsonObject &jsonObject) const;
};

#endif // GRAPHICSARCITEM_H
