#ifndef GRAPHICSLINE_H
#define GRAPHICSLINE_H

#include "graphicsitem.h"

#include <QLineF>

class GraphicsLine : public GraphicsItem
{
public:
   explicit  GraphicsLine(GraphicsItem *parent = nullptr);
    ~GraphicsLine();

    QLineF line() const;
    void setLine(const QLineF &line);
    void setLine(const QPointF &p1, const QPointF &p2)
    {
        setLine(QLineF(p1, p2));
    }
    void setLine(qreal x1, qreal y1, qreal x2, qreal y2)
    {
        setLine(QPointF(x1, y1), QPointF(x2, y2));
    }

    qreal width() const;
    void setWidth(qreal width);

    virtual bool fromJson(QString *errorString, const QJsonObject &jsonObject);
    virtual void toJson(QJsonObject &jsonObject) const;

private:
    static const QString J_LINE;
    static const QString J_WIDTH;
    QLineF m_line;
    qreal m_width;

    // QGraphicsItem interface
public:
    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // GRAPHICSLINE_H
