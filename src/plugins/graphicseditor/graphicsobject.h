#ifndef GRAPHICSOBJECT_H
#define GRAPHICSOBJECT_H

#include <QGraphicsObject>
#include <QPointF>
#include <QRectF>
#include <QPainterPath>
#include <QVector>

#include "abstractgraphicshandle.h"

#include "igraphicsitemobserver.h"

class QGraphicsItem;
class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;

class GraphicsRegularHandle;
class GraphicsBezierHandle;

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
    AbstractGraphicsHandle *handleAt(int idx);

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

    QMap<AbstractGraphicsHandle *, int> m_handleToId;
    QMap<int, AbstractGraphicsHandle *> m_idToHandle;
    GraphicsRegularHandle *addRegularHandle(int id, GraphicsHandleRole role, GraphicsHandleShape shape, const QPointF &pos = QPointF(0, 0));
    GraphicsBezierHandle *addBezierHandle(int id, const QPointF &pos = QPointF(0, 0));
    void removeHandle(int id);
    void removeHandle(AbstractGraphicsHandle *handle);
    void removeAllHandles();
    GraphicsRegularHandle *regularHandleAt(int id) const;
    GraphicsBezierHandle *bezierHandleAt(int id) const;

    void cloneTo(GraphicsObject *dst);
    static QPainterPath shapeFromPath(const QPainterPath &path, const QPen &pen);

private:
    void addAbstractHandle(AbstractGraphicsHandle *handle);

};

#endif // GRAPHICSOBJECT_H
