#ifndef GRAPHICSELLIPSEITEM_H
#define GRAPHICSELLIPSEITEM_H

#include "graphicsobject.h"

class GraphicsEllipseItem : public GraphicsObject
{
    Q_OBJECT

    Q_PROPERTY(qreal xRadius READ xRadius WRITE setXRadius NOTIFY xRadiusChanged)
    Q_PROPERTY(qreal yRadius READ yRadius WRITE setYRadius NOTIFY yRadiusChanged)

public:
    GraphicsEllipseItem(GraphicsObject *parent = 0);
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
    virtual GraphicsObject *clone();
};

#endif // GRAPHICSELLIPSEITEM_H
