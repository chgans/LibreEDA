#ifndef GRAPHICSELLIPSEITEM_H
#define GRAPHICSELLIPSEITEM_H

#include "schitem.h"

class GraphicsEllipseItem : public SchItem
{
    Q_OBJECT

    Q_PROPERTY(qreal xRadius READ xRadius WRITE setXRadius NOTIFY xRadiusChanged)
    Q_PROPERTY(qreal yRadius READ yRadius WRITE setYRadius NOTIFY yRadiusChanged)

public:
    enum HandleId {
        XRadiusHandle = 0,
        YRadiusHandle,
        NbHandles
    };

    GraphicsEllipseItem(SchItem *parent = 0);
    ~GraphicsEllipseItem();

    qreal xRadius() const;
    qreal yRadius() const;

public slots:
    void setXRadius(qreal xRadius);
    void setYRadius(qreal yRadius);

signals:
    void xRadiusChanged(qreal xRadius);
    void yRadiusChanged(qreal yRadius);

private:
    qreal m_xRadius;
    qreal m_yRadius;

    // QGraphicsItem interface
public:
    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    // GraphicsObject interface
public:
    virtual SchItem *clone();

    // QGraphicsItem interface
protected:
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    // IGraphicsItemObserver interface
public:
    virtual void itemNotification(IGraphicsObservableItem *item);
};

#endif // GRAPHICSELLIPSEITEM_H
