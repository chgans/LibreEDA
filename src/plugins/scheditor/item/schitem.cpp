#include "item/schitem.h"
#include "handle/abstractgraphicshandle.h"
#include "handle/graphicsregularhandle.h"
#include "handle/graphicsbezierhandle.h"

#include <QGraphicsTransform>

Item::Item(Item *parent):
    QGraphicsObject(parent)
{
    setPen(QPen(QBrush(QColor::fromRgb(0x80, 0x00, 0x00)), 0.5, Qt::SolidLine, Qt::RoundCap,
                Qt::RoundJoin));
    setBrush(QBrush(QColor::fromRgb(0xff, 0xff, 0xb0)));
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
}

Item::~Item()
{
}

int Item::handleCount() const
{
    return m_idToHandle.count();
}


void Item::cloneTo(Item *dst)
{
    dst->setBrush(brush());
    dst->setEnabled(isEnabled());
    dst->setFlags(flags());
    dst->setOpacity(opacity());
    dst->setPen(pen());
    dst->setPos(pos());
    dst->setRotation(rotation());
    dst->setSelected(isSelected());
    dst->setScale(scale());
    dst->setVisible(isVisible());
    dst->setZValue(zValue());
    dst->setTransform(transform());
    // Handles don't have to be copied as their creation is controlled by the item itself
}

// From Qt qgraphicsitem.cpp: qt_graphicsItem_shapeFromPath()
QPainterPath Item::shapeFromPath(const QPainterPath &path, const QPen &pen)
{
    // We unfortunately need this hack as QPainterPathStroker will set a width of 1.0
    // if we pass a value of 0.0 to QPainterPathStroker::setWidth()
    const qreal penWidthZero = qreal(0.00000001);

    if (path == QPainterPath() || pen == Qt::NoPen)
    {
        return path;
    }
    QPainterPathStroker ps;
    ps.setCapStyle(pen.capStyle());
    if (pen.widthF() <= 0.0)
    {
        ps.setWidth(penWidthZero);
    }
    else
    {
        ps.setWidth(pen.widthF());
    }
    ps.setJoinStyle(pen.joinStyle());
    ps.setMiterLimit(pen.miterLimit());
    QPainterPath p = ps.createStroke(path);
    p.addPath(path); // not needed for open path (eg, wire)
    return p;
}

void Item::updateMirroringTransform()
{
    qreal m11 = m_isXMirrored ? -1 : 1;
    qreal m22 = m_isYMirrored ? -1 : 1;
    setTransform(QTransform(m11, 0,   0,
                            0,   m22, 0,
                            0,   0,   1));
}

void Item::addAbstractHandle(Handle *handle)
{
    // could be done by abstracthandle
    addObservedItem(handle);

    m_handleToId[handle] = handle->handleId();
    m_idToHandle[handle->handleId()] = handle;
}

Handle *Item::handleAt(int idx)
{
    Q_ASSERT(idx < handleCount());
    return m_idToHandle[idx];
}

QPen Item::pen() const
{
    return m_pen;
}

QBrush Item::brush() const
{
    return m_brush;
}

bool Item::isXMirrored() const
{
    return m_isXMirrored;
}

bool Item::isYMirrored() const
{
    return m_isYMirrored;
}

// Return a list of hot spots in item's coordinate
QList<QPointF> Item::hotSpots() const
{
    QList<QPointF> points;
    for (Handle *handle : m_handleToId.keys())
    {
        Q_ASSERT(handle->parentItem() == this);
        points.append(handle->pos());
    }
    return points;
}

QList<QPointF> Item::endPoints() const
{
    return QList<QPointF>();
}

QList<QPointF> Item::midPoints() const
{
    return QList<QPointF>();
}

QList<QPointF> Item::centerPoints() const
{
    return QList<QPointF>();
}

QList<QPointF> Item::nearestPoints(QPointF pos) const
{
    Q_UNUSED(pos);
    return QList<QPointF>();
}

QList<QLineF> Item::axes() const
{
    QList<QLineF> axes;
    for (qreal angle = 0; angle < 360.0; angle += 45)
    {
        axes << QLineF::fromPolar(UINT_MAX, angle).translated(pos());
    }
    return axes;
}

void Item::setPen(const QPen &pen)
{
    if (m_pen == pen)
    {
        return;
    }

    m_pen = pen;
    prepareGeometryChange();
    m_boundingRect = QRectF();
    update();

    emit penChanged(pen);
}

void Item::setBrush(const QBrush &brush)
{
    if (m_brush == brush)
    {
        return;
    }

    m_brush = brush;
    update();

    emit brushChanged(brush);
}

void Item::setXMirrored(bool mirrored)
{
    if (m_isXMirrored == mirrored)
    {
        return;
    }
    m_isXMirrored = mirrored;
    updateMirroringTransform();
    emit xMirroredChanged();
}

void Item::setYMirrored(bool mirrored)
{
    if (m_isYMirrored == mirrored)
    {
        return;
    }
    m_isYMirrored = mirrored;
    updateMirroringTransform();
    emit yMirroredChanged();
}

RegularHandle *Item::addRegularHandle(int id, GraphicsHandleRole role,
                                                 GraphicsHandleShape shape, const QPointF &pos)
{
    RegularHandle *handle = new RegularHandle(this);

    Q_ASSERT(!m_idToHandle.keys().contains(id));

    handle->setParentGraphicsObject(this);
    handle->setHandleId(id);
    handle->setHandleRole(role);
    handle->setHandleShape(shape);
    handle->setPos(pos);

    addAbstractHandle(handle);

    return handle;
}

BezierHandle *Item::addBezierHandle(int id, const QPointF &pos)
{
    Q_UNUSED(pos);
    BezierHandle *handle = new BezierHandle(this);

    Q_ASSERT(!m_idToHandle.keys().contains(id));

    handle->setBehaviour(NormalHandleBehaviour);
    handle->setParentGraphicsObject(this);
    handle->setHandleId(id);
    //handle->setPos(pos);

    addAbstractHandle(handle);

    return handle;
}

void Item::removeHandle(int id)
{
    Q_ASSERT(m_idToHandle.keys().contains(id));

    removeHandle(m_idToHandle[id]);
}

void Item::removeHandle(Handle *handle)
{
    Q_ASSERT(m_handleToId.keys().contains(handle));

    blockItemNotification();
    int id = m_handleToId[handle];
    m_handleToId.remove(handle);
    m_idToHandle.remove(id);
    removeObservedItem(handle);
    handle->setParentItem(nullptr);
    delete handle;
    unblockItemNotification();
}

void Item::removeAllHandles()
{
    blockItemNotification();
    for (Handle *handle : m_handleToId.keys())
    {
        removeObservedItem(handle);
        handle->setParentItem(nullptr);
        delete handle;
    }
    unblockItemNotification();
    m_handleToId.clear();
    m_idToHandle.clear();
}

RegularHandle *Item::regularHandleAt(int id) const
{
    Q_ASSERT(id < handleCount());
    return static_cast<RegularHandle *>(m_idToHandle[id]);
}

BezierHandle *Item::bezierHandleAt(int id) const
{
    Q_ASSERT(id < handleCount());
    return static_cast<BezierHandle *>(m_idToHandle[id]);
}
