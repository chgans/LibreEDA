#ifndef GRAPHICSRECT_H
#define GRAPHICSRECT_H

#include "graphicsitem.h"

#include <QRectF>

class GraphicsRect : public GraphicsItem
{
public:
    GraphicsRect(GraphicsItem *parent = nullptr);
    ~GraphicsRect();

    QRectF rect() const;
    void setRect(const QRectF &rect);
    void setRect(qreal x, qreal y, qreal width, qreal height)
    {
        setRect(QRectF(x, y, width, height));
    }

    qreal cornerRadius() const;
    void setCornerRadius(qreal radius);

private:
    QRectF m_rect;
    qreal m_cornerRadius;

    // QGraphicsItem interface
public:
    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // GRAPHICSRECT_H
