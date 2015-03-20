#ifndef GRAPHICSCIRCLEITEM_H
#define GRAPHICSCIRCLEITEM_H

#include "graphicsobject.h"

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

    QRectF rect() const;
    void setRect(const QRectF &rect);
    QPen pen() const;
    void setPen(const QPen &pen);
    QBrush brush() const;
    void setBrush(const QBrush &brush);

signals:

public slots:

private:
    QRectF m_rect;
    QPen m_pen;
    QBrush m_brush;
    QMap<GraphicsHandle *, HandleId> m_handleToId;
    QMap<HandleId, GraphicsHandle *> m_idToHandle;
    void addHandle(HandleId handleId, GraphicsHandleShape shape);
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
