#ifndef GRAPHICSWIREITEM_H
#define GRAPHICSWIREITEM_H

#include "graphicsobject.h"

#include <QPointF>
#include <QList>

class QGraphicsPathItem;

class GraphicsWireItem : public GraphicsObject
{
    Q_OBJECT

    Q_PROPERTY(QList<QPointF> points READ points WRITE setPoints NOTIFY pointsChanged)

public:
    GraphicsWireItem(GraphicsObject *parent = 0);
    ~GraphicsWireItem();

    QList<QPointF> points() const;
    void addPoint(const QPointF &pos);
    void movePoint(int idx, const QPointF &pos);

public slots:
    void setPoints(QList<QPointF> points);

signals:
    void pointsChanged();

private:
    QPainterPath m_path;
    //void handleToPath();
    //void pathToHandle();

    // QGraphicsItem interface
public:
    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    // GraphicsObject interface
public:
    virtual GraphicsObject *clone();

    // IGraphicsItemObserver interface
public:
    virtual void itemNotification(IGraphicsObservableItem *item);

    // QGraphicsItem interface
protected:
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);
};

#endif // GRAPHICSWIREITEM_H
