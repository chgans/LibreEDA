#ifndef GRAPHICSCIRCLEITEM_H
#define GRAPHICSCIRCLEITEM_H

#include "graphicsobject.h"
#include "igraphicsitemobserver.h"

class GraphicsHandle;

class GraphicsCircleItem : public GraphicsObject, public IGraphicsItemObserver
{
    Q_OBJECT
public:
    enum HandleId {
        CenterHandle = 0,
        RadiusHandle,
        NbHandles
    };

    explicit GraphicsCircleItem(GraphicsObject *parent = 0);
    ~GraphicsCircleItem();

    QPen pen() const;
    void setPen(const QPen &pen);
    QBrush brush() const;
    void setBrush(const QBrush &brush);

    qreal radius() const;
    void setRadius(qreal length);

signals:

public slots:

private:
//    QPointF m_center;
//    GraphicsHandle *m_centerHandle;
    qreal m_radius;
    GraphicsHandle *m_radiusHandle;
    QPen m_pen;
    QBrush m_brush;
    QMap<GraphicsHandle *, HandleId> m_handleToId;
    QMap<HandleId, GraphicsHandle *> m_idToHandle;
    GraphicsHandle *addHandle(HandleId handleId, GraphicsHandleShape shape);
    void updateHandlesSilently();
    bool m_updatingHandles;
    mutable QRectF m_boundingRect;
    mutable QPainterPath m_shape;
    mutable bool m_dirty;
    void updateGeometry() const;
    void markDirty();

    // GraphicsObject interface
public:
    virtual GraphicsObject *clone();
    void itemNotification(IGraphicsObservableItem *item);

    // QGraphicsItem interface
public:
    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    // QGraphicsItem interface
protected:
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);
};

#endif // GRAPHICSCIRCLEITEM_H
