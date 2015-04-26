#ifndef GRAPHICSBEZIERITEM_H
#define GRAPHICSBEZIERITEM_H

#include "schitem.h"

#include <QPen>

class GraphicsBezierHandle;

class GraphicsBezierItem : public SchItem
{
public:
    GraphicsBezierItem(SchItem *parent = 0);

    QPainterPath path() const;
    void setPath(const QPainterPath &path);

    int addPoint(const QPointF &pos);
    void removePoint(int index);
    void movePoint(int index, const QPointF &pos);
    QList<QPointF> points() const;
    int pointCount() const;
    QPointF pointAt(int idx) const;

private:
    QPainterPath m_path;

    void setBoundingRectDirty();
    void computeBoundingRect() const;
    mutable bool m_boundingRectIsDirty;
    mutable QRectF m_boundingRect;

    void setShapeDirty();
    void computeShape() const;
    mutable bool m_shapeIsDirty;
    mutable QPainterPath m_shape;

    QPainterPath copyPath(const QPainterPath &src, int first, int last);

    QVector<qreal> m_px;
    QVector<qreal> m_py;
    QVector<qreal> m_c1x;
    QVector<qreal> m_c1y;
    QVector<qreal> m_c2x;
    QVector<qreal> m_c2y;

    void smoothBezier();
    void computeBezierControlPoints(const QVector<qreal> &p, QVector<qreal> &c1, QVector<qreal> &c2);

    void bezierToHandles();
    void bezierToHandle(int idx);
    void handlesToBezier();
    void handleToBezier(int idx);

    // QGraphicsItem interface
public:
    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    // GraphicsObject interface
public:
    virtual SchItem *clone();

    // IGraphicsItemObserver interface
protected:
    virtual void itemNotification(IGraphicsObservableItem *item);

    // QGraphicsItem interface
protected:
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);
};

#endif // GRAPHICSBEZIERITEM_H
