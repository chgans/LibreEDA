#ifndef GRAPHICSOBJECT_H
#define GRAPHICSOBJECT_H

#include <QGraphicsObject>
#include <QPointF>
#include <QRectF>
#include <QPainterPath>
#include <QVector>

#include "graphicseditor/graphicshandle.h"

#include "igraphicsitemobserver.h"

class QGraphicsItem;
class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;

// TODO: add properties
// TODO: AbstractPath and AbstractShape (allow to morph between AbstractXYZ impl)
// TODO: See qcad explodable concept

class GraphicsObject: public QGraphicsObject, public IGraphicsItemObserver
{
    Q_OBJECT

    Q_PROPERTY(QPen pen READ pen WRITE setPen NOTIFY penChanged)
    Q_PROPERTY(QBrush brush READ brush WRITE setBrush NOTIFY brushChanged)

public:
    GraphicsObject(GraphicsObject *parent = 0);
    virtual ~GraphicsObject();

    virtual GraphicsObject *clone() = 0;
    int handleCount() const;
    GraphicsHandle *handleAt(int idx);

    QPen pen() const;
    QBrush brush() const;

public slots:
    void setPen(const QPen &pen);
    void setBrush(const QBrush &brush);

signals:
    void penChanged(QPen pen);
    void brushChanged(QBrush brush);

protected:
    QPen m_pen;
    QBrush m_brush;
    mutable QRectF m_boundingRect;

    QMap<GraphicsHandle *, int> m_handleToId;
    QMap<int, GraphicsHandle *> m_idToHandle;
    GraphicsHandle *addHandle(int id, GraphicsHandleRole role, GraphicsHandleShape shape, const QPointF &pos = QPointF(0, 0));
    void removeHandle(int id);
    void removeHandle(GraphicsHandle *handle);
    void removeAllHandles();

    void cloneTo(GraphicsObject *dst);
    static QPainterPath shapeFromPath(const QPainterPath &path, const QPen &pen);
};

#endif // GRAPHICSOBJECT_H
