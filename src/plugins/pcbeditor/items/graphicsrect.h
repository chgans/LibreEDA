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
    void setRect(const QPointF &topLeft, const QPointF &bottomRight)
    {
        setRect(QRectF(topLeft, bottomRight));
    }

    qreal cornerRadius() const;
    void setCornerRadius(qreal radius);

    virtual bool fromJson(QString *errorString, const QJsonObject &jsonObject);
    virtual void toJson(QJsonObject &jsonObject) const;

private:
    static const QString J_RECT;
    static const QString J_CORNER_RADIUS;

    QRectF m_rect;
    qreal m_cornerRadius;

    // QGraphicsItem interface
public:
    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // GRAPHICSRECT_H
