#ifndef GRAPHICSPOLYGONITEM_H
#define GRAPHICSPOLYGONITEM_H

#include "graphicsobject.h"
#include "igraphicsitemobserver.h"

class GraphicsHandle;

class GraphicsPolygonItem : public GraphicsObject, public IGraphicsItemObserver
{
    Q_OBJECT

    Q_PROPERTY(Qt::FillRule fillRule READ fillRule WRITE setFillRule NOTIFY fillRuleChanged)
    Q_PROPERTY(QPolygonF polygon READ polygon WRITE setPolygon NOTIFY polygonChanged)

public:
    GraphicsPolygonItem(GraphicsObject *parent = 0);
    ~GraphicsPolygonItem();

    Qt::FillRule fillRule() const;
    QPolygonF polygon() const;

public slots:
    void setFillRule(Qt::FillRule fillRule);
    void setPolygon(QPolygonF polygon);
    void addPoint(const QPointF &pos);
    void movePoint(int idx, const QPointF &pos);

signals:
    void fillRuleChanged(Qt::FillRule fillRule);
    void polygonChanged(QPolygonF polygon);

private:
    Qt::FillRule m_fillRule;
    QPolygonF m_polygon;
    QList<GraphicsHandle *> m_handles;
    GraphicsHandle *addHandle(const QPointF &pos);
    void removeHandle(int idx);
    void handleToPolygon();
    void polygonToHandle();

    // QGraphicsItem interface
public:
    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    // GraphicsObject interface
public:
    virtual GraphicsObject *clone();

    // QGraphicsItem interface
protected:
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    // IGraphicsItemObserver interface
public:
    virtual void itemNotification(IGraphicsObservableItem *item);
};

#endif // GRAPHICSPOLYGONITEM_H
