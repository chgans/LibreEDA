#include "item/graphicsbezieritem.h"
#include "handle/graphicsbezierhandle.h"
#include "schscene.h"
#include "schview.h"

#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QDebug>
#include <QJsonObject>

#include <QLoggingCategory>
Q_DECLARE_LOGGING_CATEGORY(GraphicsBezierItemLog)
Q_LOGGING_CATEGORY(GraphicsBezierItemLog, "graphics.bezier.item")
#define DEBUG() qCDebug(GraphicsBezierItemLog)
#define DEBUG() qCDebug(GraphicsBezierItemLog)
#define WARNING() qCDebug(GraphicsBezierItemLog)

/*
 * TODO:
 *  - Add a beginMove/endMove, while moving we don't have to recompute shape
 */

GraphicsBezierItem::GraphicsBezierItem(SchItem *parent):
    SchItem(parent)
{
    setFlag(QGraphicsItem::ItemUsesExtendedStyleOption, true);
}

QPainterPath GraphicsBezierItem::path() const
{
    return m_path;
}

void GraphicsBezierItem::setPath(const QPainterPath &path)
{
    m_path = path;
    setShapeDirty();
    setBoundingRectDirty();
}

// pos is in local coordinate
int GraphicsBezierItem::addPoint(const QPointF &pos)
{
    DEBUG() << "About to insert BezierHandle #" << handleCount() + 1 << "at" << pos;

    int newId = pointCount();
    GraphicsBezierHandle *newHandle = addBezierHandle(newId);
    newHandle->setNodePos(pos);

    if (newId == 0) {
        newHandle->setFirst(true);
        newHandle->setLast(true);
    }
    else if (!(qFuzzyCompare(m_px.last(), pos.x()) &&
               qFuzzyCompare(m_py.last(), pos.y()))) {
        // Update the first/last status
        GraphicsBezierHandle *prevHandle = bezierHandleAt(newId - 1);
        prevHandle->setLast(false);
        newHandle->setLast(true);
    }
    else {
        WARNING() << "Cannot add a point at same pos as current one";
        removeHandle(newHandle);
        return -1;
    }

    m_px.append(pos.x());
    m_py.append(pos.y());
    if (!newHandle->isFirst()) {
        m_c2x.append(0);
        m_c2y.append(0);/*
    }
    if (!newHandle->isLast()) {*/
        m_c1x.append(0);
        m_c1y.append(0);
    }


    blockItemNotification();
    smoothBezier();
    bezierToHandles();
    unblockItemNotification();

    DEBUG() << "Point fully inserted";

    return pointCount() - 1;
}

void GraphicsBezierItem::removePoint(int index)
{
    Q_ASSERT(index < pointCount());

    // Remove data points
    m_px.removeAt(index);
    m_py.removeAt(index);
    if (index != 0) {
        m_c2x.removeAt(index-1);
        m_c2y.removeAt(index-1); /*
    }
    if (!handle->isLast()) {*/
        m_c1x.removeAt(index-1);
        m_c1y.removeAt(index-1);
    }

    // Remove handle
    GraphicsBezierHandle *handle = bezierHandleAt(index);
    bool firstRemoved = index == 0;
    bool lastRemoved = index == (handleCount() - 1);
    removeHandle(handle);

    // Update first/last status: FIXME: now we use index in abstracthandle
    if (firstRemoved && handleCount() != 0)
        bezierHandleAt(handleCount() - 1)->setFirst(true);
    if (lastRemoved && handleCount() != 0)
        bezierHandleAt(handleCount() - 1)->setLast(true);

    // Update handles
    smoothBezier();
    blockItemNotification();
    bezierToHandles();
    unblockItemNotification();
}

void GraphicsBezierItem::movePoint(int index, const QPointF &pos)
{
    Q_ASSERT(index < pointCount());

    m_px[index] = pos.x();
    m_py[index] = pos.y();

    blockItemNotification();
    smoothBezier();
    bezierToHandles();
    unblockItemNotification();
}

QPainterPath GraphicsBezierItem::copyPath(const QPainterPath &src, int first, int last)
{
    QPainterPath dst;
    int i = first;
    while (i <= (last*3)) {
        if (i == 0) {
            QPainterPath::Element elt = src.elementAt(i);
            dst.moveTo(elt.x, elt.y);
            i++;
        }
        else {
            QPainterPath::Element c1 = src.elementAt(i);
            QPainterPath::Element c2 = src.elementAt(i+1);
            QPainterPath::Element pt = src.elementAt(i+2);
            dst.cubicTo(c1.x, c1.y, c2.x, c2.y, pt.x, pt.y);
            i+= 3;
        }
    }
    //qDebug() << dst;
    return dst;
}

void GraphicsBezierItem::smoothBezier()
{

    // TODO: don't always rebuild the whole path
    // If the pointCount() hasn't change use moveElement
    m_path = QPainterPath();
    if (pointCount() == 0)
        return;

    setShapeDirty();
    setBoundingRectDirty();

    if (pointCount() >= 2) {
        computeBezierControlPoints(m_px, m_c1x, m_c2x);
        computeBezierControlPoints(m_py, m_c1y, m_c2y);
    }

    m_path.moveTo(m_px[0], m_py[0]);
    for (int i = 1; i < m_px.size(); i++) {
        qreal c1x = m_c1x[i-1];
        qreal c1y = m_c1y[i-1];
        qreal c2x = m_c2x[i-1];
        qreal c2y = m_c2y[i-1];
        qreal px  = m_px[i];
        qreal py  = m_py[i];
        m_path.cubicTo(c1x, c1y, c2x, c2y, px, py);
    }

    DEBUG() << m_path;
}

// https://www.particleincell.com/2012/bezier-splines/
void GraphicsBezierItem::computeBezierControlPoints(const QVector<qreal> &p, QVector<qreal> &c1, QVector<qreal> &c2)
{
    int n = p.size() - 1;
    int i;
    QVector<qreal> a(n), b(n), c(n), r(n);

    /* left most segment */
    a[0] = 0;
    b[0] = 2;
    c[0] = 1;
    r[0] = p[0] + 2*p[1];

    /* internal segments */
    for (i = 1; i < n - 1; i++)
    {
        a[i] = 1;
        b[i] = 4;
        c[i] = 1;
        r[i] = 4*p[i] + 2*p[i+1];
    }

    /* right segment */
    a[n-1] = 2;
    b[n-1] = 7;
    c[n-1] = 0;
    r[n-1] = 8*p[n-1] + p[n];

    /* solves Ax=b with the Thomas algorithm (from Wikipedia) */
    for (i = 1; i < n; i++)
    {
        qreal m = a[i] / b[i-1];
        b[i] = b[i] - m*c[i-1];
        r[i] = r[i] - m*r[i-1];
    }

    /* compute p1 */
    c1[n-1] = r[n-1]/b[n-1];
    for (i = n-2; i >= 0; --i)
        c1[i] = (r[i] - c[i] * c1[i+1]) / b[i];

    /* compute p2 */
    for (i = 0; i < n-1; i++)
        c2[i] = 2*p[i+1] - c1[i+1];
    c2[n-1] = 0.5*(p[n] + c1[n-1]);
}

//
void GraphicsBezierItem::bezierToHandles()
{
    Q_ASSERT(pointCount() == handleCount());

    if (pointCount() == 0)
        return;

    Q_ASSERT(bezierHandleAt(0)->isFirst());
    Q_ASSERT(bezierHandleAt(handleCount() - 1)->isLast());

    for (int i = 0; i < pointCount(); i++) {
        bezierToHandle(i);
    }
}

void GraphicsBezierItem::bezierToHandle(int idx)
{
    GraphicsBezierHandle *handle = bezierHandleAt(idx);

    handle->setNodePos(m_px[idx], m_py[idx]);

    if (!handle->isFirst())
        handle->setControl1Pos(m_c2x[idx-1], m_c2y[idx-1]);

    if (!handle->isLast())
        handle->setControl2Pos(m_c1x[idx], m_c1y[idx]);
}

void GraphicsBezierItem::handlesToBezier()
{
    for (int i = 0; i < pointCount(); i++)
        handleToBezier(i);
}

void GraphicsBezierItem::handleToBezier(int idx)
{
    Q_ASSERT(pointCount() == handleCount());

    if (pointCount() == 0)
        return;

    GraphicsBezierHandle *handle = bezierHandleAt(idx);

    m_px[idx] = handle->nodePos().x();
    m_py[idx] = handle->nodePos().y();

    if (!handle->isFirst()) {
        m_c2x[idx-1] = handle->control1Pos().x();
        m_c2y[idx-1] = handle->control1Pos().y();
    }

    if (!handle->isLast()) {
        m_c1x[idx] = handle->control2Pos().x();
        m_c1y[idx] = handle->control2Pos().y();
    }
}

QPointF GraphicsBezierItem::pointAt(int idx) const
{
    Q_ASSERT(idx < pointCount());
    return QPointF(m_px[idx], m_py[idx]);
}

QList<QPointF> GraphicsBezierItem::points() const
{
    QList<QPointF> result;
    for (int i = 0; i < pointCount(); i++) {
        result.append(pointAt(i));
    }
    return result;
}

int GraphicsBezierItem::pointCount() const
{
    return m_px.size();
}

void GraphicsBezierItem::setBoundingRectDirty()
{
    prepareGeometryChange();
    m_boundingRectIsDirty = true;
}

void GraphicsBezierItem::computeBoundingRect() const
{
    qreal extra = pen().widthF()/2.0;
    m_boundingRect = m_path.boundingRect().adjusted(-extra, -extra, +extra, +extra);
    m_boundingRectIsDirty = false;
}

void GraphicsBezierItem::setShapeDirty()
{
    m_shapeIsDirty = true;
}

void GraphicsBezierItem::computeShape() const
{
    QPainterPathStroker stroker;
    stroker.setWidth(pen().widthF());
    stroker.setCapStyle(pen().capStyle());
    stroker.setJoinStyle(pen().joinStyle());
    stroker.setMiterLimit(pen().miterLimit());
    m_shape = stroker.createStroke(m_path);
    m_shapeIsDirty = false;
}

QRectF GraphicsBezierItem::boundingRect() const
{
    if (m_boundingRectIsDirty)
        computeBoundingRect();
    return m_boundingRect;
}

QPainterPath GraphicsBezierItem::shape() const
{
    if (m_shapeIsDirty)
        computeShape();
    return m_shape;
}

void GraphicsBezierItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    // TBD: exposedRect seems to be always equal boudingRect()
    // http://thesmithfam.org/blog/2007/02/03/qt-improving-qgraphicsview-performance/
    if (!shape().intersects(option->exposedRect))
        return;

    // From same web page
    painter->setClipRect(option->exposedRect);

    painter->setPen(pen());
    painter->setBrush(Qt::NoBrush);
    painter->drawPath(m_path);
}

SchItem *GraphicsBezierItem::clone()
{
    GraphicsBezierItem *item = new GraphicsBezierItem();
    SchItem::cloneTo(item);
    foreach (const QPointF &point, points()) {
        item->addPoint(point);
    }
    return item;
}

bool GraphicsBezierItem::fromJson(QString *errorString, const QJsonObject &jsonObject)
{
    Q_UNUSED(errorString);
    Q_UNUSED(jsonObject);
    return true;
}

void GraphicsBezierItem::toJson(QJsonObject &jsonObject) const
{
    Q_UNUSED(jsonObject);
}

void GraphicsBezierItem::itemNotification(IGraphicsObservableItem *item)
{
    Q_UNUSED(item);
    blockItemNotification();
    handlesToBezier();
    if (m_px.size() >= 2) {
        smoothBezier();
        bezierToHandles();
    }
    unblockItemNotification();
}


QVariant GraphicsBezierItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemSelectedHasChanged) {
        foreach (AbstractGraphicsHandle *handle, m_handleToId.keys()) {
            handle->setVisible(isSelected());
        }
    }
    return value;
}
