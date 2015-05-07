#ifndef GRAPHICSARCITEM_H
#define GRAPHICSARCITEM_H

#include "schitem.h"

// TODO: Pie vs chord vs whole shape
// TODO: keep ratio (ellipse vs circle)

class GraphicsArcItem : public SchItem
{

public:
    enum HandleId {
        XRadiusHandle = 0,
        YRadiusHandle,
        StartAngleHandle,
        SpanAngleHandle
    };

    GraphicsArcItem(SchItem *parent = 0);
    GraphicsArcItem(qreal xRadius, qreal yRadius, SchItem *parent = 0);
    GraphicsArcItem(qreal xRadius, qreal yRadius, int startAngle, int spanAngle, SchItem *parent = 0);
    ~GraphicsArcItem();

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
    virtual void itemNotification(IGraphicsObservableItem *item);

    // SchItem interface
public:
    virtual SchItem *clone();
    virtual bool fromJson(QString *errorString, const QJsonObject &jsonObject);
    virtual void toJson(QJsonObject &jsonObject) const;
};

#endif // GRAPHICSARCITEM_H
