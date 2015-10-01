#ifndef GRAPHICSOBJECT_H
#define GRAPHICSOBJECT_H

#include <QGraphicsObject>
#include <QPointF>
#include <QRectF>
#include <QPainterPath>
#include <QVector>

#include "handle/abstractgraphicshandle.h"

#include "item/igraphicsitemobserver.h"

class QGraphicsItem;
class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;

class GraphicsRegularHandle;
class GraphicsBezierHandle;

// TODO: add properties
// TODO: AbstractPath and AbstractShape (allow to morph between AbstractXYZ impl)
// TODO: See qcad explodable concept

class SchItem: public QGraphicsObject, public IGraphicsItemObserver
{
    Q_OBJECT

    Q_PROPERTY(QPen pen READ pen WRITE setPen NOTIFY penChanged)
    Q_PROPERTY(QBrush brush READ brush WRITE setBrush NOTIFY brushChanged)

public:
    SchItem(SchItem *parent = 0);
    virtual ~SchItem();

    virtual SchItem *clone() = 0;
    int handleCount() const;
    AbstractGraphicsHandle *handleAt(int idx);

    QPen pen() const;
    QBrush brush() const;

    virtual bool fromJson(QString *errorString, const QJsonObject &jsonObject);
    virtual void toJson(QJsonObject &jsonObject) const;

    virtual QList<QPointF> hotSpots() const;
    virtual QList<QPointF> endPoints() const;
    virtual QList<QPointF> midPoints() const;
    virtual QList<QPointF> centerPoints() const;
    virtual QList<QPointF> nearestPoints(QPointF pos) const;

    virtual QList<QLineF> axes() const;

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

    void cloneTo(SchItem *dst);
    static QPainterPath shapeFromPath(const QPainterPath &path, const QPen &pen);

private:
    static const QString J_POSITION;
    static const QString J_ZVALUE;
    static const QString J_ROTATION;
    static const QString J_LOCKED;
    static const QString J_PEN;
    static const QString J_BRUSH;

    void addAbstractHandle(AbstractGraphicsHandle *handle);

};

#endif // GRAPHICSOBJECT_H
