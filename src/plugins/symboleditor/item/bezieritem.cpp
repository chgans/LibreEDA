#include "item/bezieritem.h"
#include "handle/bezierhandle.h"

#include <QPainter>
#include <QStyleOptionGraphicsItem>

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

namespace SymbolEditor
{

    BezierItem::BezierItem(Item *parent):
        Item(parent)
    {
        setFlag(QGraphicsItem::ItemUsesExtendedStyleOption, true);
    }

    QPainterPath BezierItem::path() const
    {
        return m_path;
    }

    void BezierItem::setPath(const QPainterPath &path)
    {
        m_path = path;
        setShapeDirty();
        setBoundingRectDirty();
    }

    // pos is in local coordinate
    int BezierItem::addPoint(const QPointF &pos)
    {
        DEBUG() << "About to insert BezierHandle #" << handleCount() + 1 << "at" << pos;

        int newId = pointCount();
        BezierHandle *newHandle = addBezierHandle(newId);
        newHandle->setNodePos(pos);

        if (newId == 0)
        {
            newHandle->setFirst(true);
            newHandle->setLast(true);
        }
        else if (!(qFuzzyCompare(m_px.last(), pos.x()) &&
                   qFuzzyCompare(m_py.last(), pos.y())))
        {
            // Update the first/last status
            BezierHandle *prevHandle = bezierHandleAt(newId - 1);
            prevHandle->setLast(false);
            newHandle->setLast(true);
        }
        else
        {
            WARNING() << "Cannot add a point at same pos as current one";
            removeHandle(newHandle);
            return -1;
        }

        m_px.append(pos.x());
        m_py.append(pos.y());
        if (!newHandle->isFirst())
        {
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

    void BezierItem::removePoint(int index)
    {
        Q_ASSERT(index < pointCount());

        // Remove data points
        m_px.removeAt(index);
        m_py.removeAt(index);
        if (index != 0)
        {
            m_c2x.removeAt(index - 1);
            m_c2y.removeAt(index - 1); /*
    }
    if (!handle->isLast()) {*/
            m_c1x.removeAt(index - 1);
            m_c1y.removeAt(index - 1);
        }

        // Remove handle
        BezierHandle *handle = bezierHandleAt(index);
        bool firstRemoved = index == 0;
        bool lastRemoved = index == (handleCount() - 1);
        removeHandle(handle);

        // Update first/last status: FIXME: now we use index in abstracthandle
        if (firstRemoved && handleCount() != 0)
        {
            bezierHandleAt(handleCount() - 1)->setFirst(true);
        }
        if (lastRemoved && handleCount() != 0)
        {
            bezierHandleAt(handleCount() - 1)->setLast(true);
        }

        // Update handles
        smoothBezier();
        blockItemNotification();
        bezierToHandles();
        unblockItemNotification();
    }

    void BezierItem::movePoint(int index, const QPointF &pos)
    {
        Q_ASSERT(index < pointCount());

        m_px[index] = pos.x();
        m_py[index] = pos.y();

        blockItemNotification();
        smoothBezier();
        bezierToHandles();
        unblockItemNotification();
    }

    QPainterPath BezierItem::copyPath(const QPainterPath &src, int first, int last)
    {
        QPainterPath dst;
        int i = first;
        while (i <= (last * 3))
        {
            if (i == 0)
            {
                QPainterPath::Element elt = src.elementAt(i);
                dst.moveTo(elt.x, elt.y);
                i++;
            }
            else
            {
                QPainterPath::Element c1 = src.elementAt(i);
                QPainterPath::Element c2 = src.elementAt(i + 1);
                QPainterPath::Element pt = src.elementAt(i + 2);
                dst.cubicTo(c1.x, c1.y, c2.x, c2.y, pt.x, pt.y);
                i += 3;
            }
        }
        return dst;
    }

    void BezierItem::smoothBezier()
    {

        // TODO: don't always rebuild the whole path
        // If the pointCount() hasn't change use moveElement
        m_path = QPainterPath();
        if (pointCount() == 0)
        {
            return;
        }

        setShapeDirty();
        setBoundingRectDirty();

        if (pointCount() >= 2)
        {
            computeBezierControlPoints(m_px, m_c1x, m_c2x);
            computeBezierControlPoints(m_py, m_c1y, m_c2y);
        }

        m_path.moveTo(m_px[0], m_py[0]);
        for (int i = 1; i < m_px.size(); i++)
        {
            qreal c1x = m_c1x[i - 1];
            qreal c1y = m_c1y[i - 1];
            qreal c2x = m_c2x[i - 1];
            qreal c2y = m_c2y[i - 1];
            qreal px  = m_px[i];
            qreal py  = m_py[i];
            m_path.cubicTo(c1x, c1y, c2x, c2y, px, py);
        }

        DEBUG() << m_path;
    }

    // https://www.particleincell.com/2012/bezier-splines/
    void BezierItem::computeBezierControlPoints(const QVector<qreal> &p, QVector<qreal> &c1,
                                                QVector<qreal> &c2)
    {
        int n = p.size() - 1;
        int i;
        QVector<qreal> a(n), b(n), c(n), r(n);

        /* left most segment */
        a[0] = 0;
        b[0] = 2;
        c[0] = 1;
        r[0] = p[0] + 2 * p[1];

        /* internal segments */
        for (i = 1; i < n - 1; i++)
        {
            a[i] = 1;
            b[i] = 4;
            c[i] = 1;
            r[i] = 4 * p[i] + 2 * p[i + 1];
        }

        /* right segment */
        a[n - 1] = 2;
        b[n - 1] = 7;
        c[n - 1] = 0;
        r[n - 1] = 8 * p[n - 1] + p[n];

        /* solves Ax=b with the Thomas algorithm (from Wikipedia) */
        for (i = 1; i < n; i++)
        {
            qreal m = a[i] / b[i - 1];
            b[i] = b[i] - m * c[i - 1];
            r[i] = r[i] - m * r[i - 1];
        }

        /* compute p1 */
        c1[n - 1] = r[n - 1] / b[n - 1];
        for (i = n - 2; i >= 0; --i)
        {
            c1[i] = (r[i] - c[i] * c1[i + 1]) / b[i];
        }

        /* compute p2 */
        for (i = 0; i < n - 1; i++)
        {
            c2[i] = 2 * p[i + 1] - c1[i + 1];
        }
        c2[n - 1] = 0.5 * (p[n] + c1[n - 1]);
    }

    //
    void BezierItem::bezierToHandles()
    {
        Q_ASSERT(pointCount() == handleCount());

        if (pointCount() == 0)
        {
            return;
        }

        Q_ASSERT(bezierHandleAt(0)->isFirst());
        Q_ASSERT(bezierHandleAt(handleCount() - 1)->isLast());

        for (int i = 0; i < pointCount(); i++)
        {
            bezierToHandle(i);
        }
    }

    void BezierItem::bezierToHandle(int idx)
    {
        BezierHandle *handle = bezierHandleAt(idx);

        handle->setNodePos(m_px[idx], m_py[idx]);

        if (!handle->isFirst())
        {
            handle->setControl1Pos(m_c2x[idx - 1], m_c2y[idx - 1]);
        }

        if (!handle->isLast())
        {
            handle->setControl2Pos(m_c1x[idx], m_c1y[idx]);
        }
    }

    void BezierItem::handlesToBezier()
    {
        for (int i = 0; i < pointCount(); i++)
        {
            handleToBezier(i);
        }
    }

    void BezierItem::handleToBezier(int idx)
    {
        Q_ASSERT(pointCount() == handleCount());

        if (pointCount() == 0)
        {
            return;
        }

        BezierHandle *handle = bezierHandleAt(idx);

        m_px[idx] = handle->nodePos().x();
        m_py[idx] = handle->nodePos().y();

        if (!handle->isFirst())
        {
            m_c2x[idx - 1] = handle->control1Pos().x();
            m_c2y[idx - 1] = handle->control1Pos().y();
        }

        if (!handle->isLast())
        {
            m_c1x[idx] = handle->control2Pos().x();
            m_c1y[idx] = handle->control2Pos().y();
        }
    }

    QPointF BezierItem::pointAt(int idx) const
    {
        Q_ASSERT(idx < pointCount());
        return QPointF(m_px[idx], m_py[idx]);
    }

    QList<QPointF> BezierItem::points() const
    {
        QList<QPointF> result;
        for (int i = 0; i < pointCount(); i++)
        {
            result.append(pointAt(i));
        }
        return result;
    }

    int BezierItem::pointCount() const
    {
        return m_px.size();
    }

    void BezierItem::setBoundingRectDirty()
    {
        prepareGeometryChange();
        m_boundingRectIsDirty = true;
    }

    void BezierItem::computeBoundingRect() const
    {
        qreal extra = pen().widthF() / 2.0;
        m_boundingRect = m_path.boundingRect().adjusted(-extra, -extra, +extra, +extra);
        m_boundingRectIsDirty = false;
    }

    void BezierItem::setShapeDirty()
    {
        m_shapeIsDirty = true;
    }

    void BezierItem::computeShape() const
    {
        QPainterPathStroker stroker;
        stroker.setWidth(pen().widthF());
        stroker.setCapStyle(pen().capStyle());
        stroker.setJoinStyle(pen().joinStyle());
        stroker.setMiterLimit(pen().miterLimit());
        m_shape = stroker.createStroke(m_path);
        m_shapeIsDirty = false;
    }

    QRectF BezierItem::boundingRect() const
    {
        if (m_boundingRectIsDirty)
        {
            computeBoundingRect();
        }
        return m_boundingRect;
    }

    QPainterPath BezierItem::shape() const
    {
        if (m_shapeIsDirty)
        {
            computeShape();
        }
        return m_shape;
    }

    void BezierItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                           QWidget *widget)
    {
        Q_UNUSED(widget);
        // TBD: exposedRect seems to be always equal boudingRect()
        // http://thesmithfam.org/blog/2007/02/03/qt-improving-qgraphicsview-performance/
        if (!shape().intersects(option->exposedRect))
        {
            return;
        }

        // From same web page
        painter->setClipRect(option->exposedRect);

        painter->setPen(pen());
        painter->setBrush(Qt::NoBrush);
        painter->drawPath(m_path);
    }

    Item *BezierItem::clone()
    {
        BezierItem *item = new BezierItem();
        Item::cloneTo(item);
        for (const QPointF &point : points())
        {
            item->addPoint(point);
        }
        return item;
    }

    void BezierItem::setProperty(quint64 id, const QVariant &value)
    {
        Item::setProperty(id, value);
    }

    void BezierItem::itemNotification(IObservableItem *item)
    {
        Q_UNUSED(item);
        blockItemNotification();
        handlesToBezier();
        if (m_px.size() >= 2)
        {
            smoothBezier();
            bezierToHandles();
        }
        unblockItemNotification();
    }


    QVariant BezierItem::itemChange(QGraphicsItem::GraphicsItemChange change,
                                    const QVariant &value)
    {
        if (change == QGraphicsItem::ItemSelectedHasChanged)
        {
            for (Handle *handle : m_handleToId.keys())
            {
                handle->setVisible(isSelected());
            }
        }
        return value;
    }

}
