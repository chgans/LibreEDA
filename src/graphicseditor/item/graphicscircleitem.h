#ifndef GRAPHICSCIRCLEITEM_H
#define GRAPHICSCIRCLEITEM_H

#include "graphicsobject.h"
#include "igraphicsitemobserver.h"

class GraphicsHandle;

class GraphicsCircleItem : public GraphicsObject, public IGraphicsItemObserver
{
    Q_OBJECT

    Q_PROPERTY(qreal radius READ radius WRITE setRadius NOTIFY radiusChanged)

public:
    enum HandleId {
        CenterHandle = 0,
        RadiusHandle,
        NbHandles
    };

    explicit GraphicsCircleItem(GraphicsObject *parent = 0);
    ~GraphicsCircleItem();

    qreal radius() const;

public slots:
    void setRadius(qreal length);

signals:
    void radiusChanged();

private:
    qreal m_radius;
    QMap<GraphicsHandle *, HandleId> m_handleToId;
    QMap<HandleId, GraphicsHandle *> m_idToHandle;
    GraphicsHandle *addHandle(HandleId handleId);

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
