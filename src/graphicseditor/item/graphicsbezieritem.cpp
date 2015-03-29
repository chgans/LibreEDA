#include "graphicsbezieritem.h"
#include "graphicsscene.h"
#include "graphicsview.h"
#include "graphicshandle.h"

#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QDebug>

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

GraphicsBezierItem::GraphicsBezierItem(GraphicsObject *parent):
    GraphicsObject(parent)
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
    DEBUG() << "About to insert GraphicsPathPoint #" << childItems().count() + 1 << "at" << pos;

    GraphicsPathPoint *pathPoint = new GraphicsPathPoint(this);

    if (pointCount() == 0) {
        pathPoint->setFirst(true);
        pathPoint->setLast(true);
    }
    else if (!(qFuzzyCompare(m_px.last(), pos.x()) &&
               qFuzzyCompare(m_py.last(), pos.y()))) {
        // Update the first/last status
        m_pathPoints.last()->setLast(false);
        pathPoint->setLast(true);
    }
    else {
        WARNING() << "Cannot add a point at same pos as current one";
        delete pathPoint;
        return -1;
    }

    m_px.append(pos.x());
    m_py.append(pos.y());
    if (!pathPoint->isFirst()) {
        m_c2x.append(0);
        m_c2y.append(0);/*
    }
    if (!pathPoint->isLast()) {*/
        m_c1x.append(0);
        m_c1y.append(0);
    }

    m_pathPoints.append(pathPoint);

    blockItemNotification();
    addObservedItem(pathPoint);
    smoothBezier();
    bezierToPathPoints();
    unblockItemNotification();

    DEBUG() << "Point fully inserted";

    return pointCount() - 1;
}

void GraphicsBezierItem::removePoint(int index)
{
    Q_ASSERT(index < pointCount());

    m_px.removeAt(index);
    m_py.removeAt(index);
    if (index != 0) {
        m_c2x.removeAt(index-1);
        m_c2y.removeAt(index-1); /*
    }
    if (!pathPoint->isLast()) {*/
        m_c1x.removeAt(index-1);
        m_c1y.removeAt(index-1);
    }

    GraphicsPathPoint *pathPoint = m_pathPoints.at(index);
    bool firstRemoved = index == 0;
    bool lastRemoved = index == (m_pathPoints.count() - 1);
    blockItemNotification();
    removeObservedItem(pathPoint);
    m_pathPoints.removeAt(index);
    if (firstRemoved && !m_pathPoints.isEmpty())
        m_pathPoints.first()->setFirst(true);
    if (lastRemoved && !m_pathPoints.isEmpty())
        m_pathPoints.last()->setLast(true);
    smoothBezier();
    bezierToPathPoints();
    unblockItemNotification();
}

void GraphicsBezierItem::movePoint(int index, const QPointF &pos)
{
    Q_ASSERT(index < pointCount());

    m_px[index] = pos.x();
    m_py[index] = pos.y();

    blockItemNotification();
    smoothBezier();
    bezierToPathPoints();
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
void GraphicsBezierItem::bezierToPathPoints()
{
    Q_ASSERT(pointCount() == m_pathPoints.count());

    if (pointCount() == 0)
        return;

    Q_ASSERT(m_pathPoints.first()->isFirst());
    Q_ASSERT(m_pathPoints.last()->isLast());

    for (int i = 0; i < pointCount(); i++) {
        bezierToPathPoint(i);
    }
}

void GraphicsBezierItem::bezierToPathPoint(int idx)
{
    GraphicsPathPoint *pathPoint = m_pathPoints.at(idx);

    pathPoint->setNodePos(m_px[idx], m_py[idx]);

    if (!pathPoint->isFirst())
        pathPoint->setControl1Pos(m_c2x[idx-1], m_c2y[idx-1]);

    if (!pathPoint->isLast())
        pathPoint->setControl2Pos(m_c1x[idx], m_c1y[idx]);
}

void GraphicsBezierItem::pathPointsToBezier()
{
    for (int i = 0; i < pointCount(); i++)
        pathPointToBezier(i);
}

void GraphicsBezierItem::pathPointToBezier(int idx)
{
    Q_ASSERT(pointCount() == m_pathPoints.count());

    if (pointCount() == 0)
        return;

    GraphicsPathPoint *pathPoint = m_pathPoints.at(idx);

    m_px[idx] = pathPoint->nodePos().x();
    m_py[idx] = pathPoint->nodePos().y();

    if (!pathPoint->isFirst()) {
        m_c2x[idx-1] = pathPoint->control1Pos().x();
        m_c2y[idx-1] = pathPoint->control1Pos().y();
    }

    if (!pathPoint->isLast()) {
        m_c1x[idx] = pathPoint->control2Pos().x();
        m_c1y[idx] = pathPoint->control2Pos().y();
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

GraphicsObject *GraphicsBezierItem::clone()
{
    GraphicsBezierItem *item = new GraphicsBezierItem();
    GraphicsObject::cloneTo(item);
    foreach (const QPointF &point, points()) {
        item->addPoint(point);
    }
    return item;
}

void GraphicsBezierItem::itemNotification(IGraphicsObservableItem *item)
{
    blockItemNotification();
    pathPointsToBezier();
    if (m_px.size() >= 2) {
        smoothBezier();
        bezierToPathPoints();
    }
    unblockItemNotification();
}


QVariant GraphicsBezierItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemSelectedHasChanged) {
        foreach (QGraphicsItem *h, childItems()) {
            h->setVisible(isSelected());
        }
    }
    return value;
}
