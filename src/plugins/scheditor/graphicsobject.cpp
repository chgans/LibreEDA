#include "graphicsobject.h"
#include "abstractgraphicshandle.h"
#include "graphicsregularhandle.h"
#include "graphicsbezierhandle.h"

#include <QDebug>

SchItem::SchItem(SchItem *parent):
    QGraphicsObject(parent)
{
    setPen(QPen(QBrush(QColor::fromRgb(0x80, 0x00, 0x00)), 0.5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    setBrush(QBrush(QColor::fromRgb(0xff, 0xff, 0xb0)));
    setFlags(QGraphicsItem::ItemIsSelectable|QGraphicsItem::ItemIsMovable);
}

SchItem::~SchItem()
{
}

int SchItem::handleCount() const
{
    return m_idToHandle.count();
}


void SchItem::cloneTo(SchItem *dst)
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
    // Handles don't have to be copied as their creation is controlled by the item itself
}

// From Qt qgraphicsitem.cpp: qt_graphicsItem_shapeFromPath()
QPainterPath SchItem::shapeFromPath(const QPainterPath &path, const QPen &pen)
{
    // We unfortunately need this hack as QPainterPathStroker will set a width of 1.0
    // if we pass a value of 0.0 to QPainterPathStroker::setWidth()
    const qreal penWidthZero = qreal(0.00000001);

    if (path == QPainterPath() || pen == Qt::NoPen)
        return path;
    QPainterPathStroker ps;
    ps.setCapStyle(pen.capStyle());
    if (pen.widthF() <= 0.0)
        ps.setWidth(penWidthZero);
    else
        ps.setWidth(pen.widthF());
    ps.setJoinStyle(pen.joinStyle());
    ps.setMiterLimit(pen.miterLimit());
    QPainterPath p = ps.createStroke(path);
    p.addPath(path); // not needed for open path (eg, wire)
    return p;
}

void SchItem::addAbstractHandle(AbstractGraphicsHandle *handle)
{
    // could be done by abstracthandle
    addObservedItem(handle);

    m_handleToId[handle] = handle->handleId();
    m_idToHandle[handle->handleId()] = handle;
}

AbstractGraphicsHandle *SchItem::handleAt(int idx)
{
    Q_ASSERT(idx < handleCount());
    return m_idToHandle[idx];
}

QPen SchItem::pen() const
{
    return m_pen;
}

QBrush SchItem::brush() const
{
    return m_brush;
}

void SchItem::setPen(const QPen &pen)
{
    if (m_pen == pen)
        return;

    m_pen = pen;
    prepareGeometryChange();
    m_boundingRect = QRectF();
    update();

    emit penChanged(pen);
}

void SchItem::setBrush(const QBrush &brush)
{
    if (m_brush == brush)
        return;

    m_brush = brush;
    update();

    emit brushChanged(brush);
}

GraphicsRegularHandle *SchItem::addRegularHandle(int id, GraphicsHandleRole role, GraphicsHandleShape shape, const QPointF &pos)
{
    GraphicsRegularHandle *handle = new GraphicsRegularHandle(this);

    Q_ASSERT(!m_idToHandle.keys().contains(id));

    handle->setParentGraphicsObject(this);
    handle->setHandleId(id);
    handle->setHandleRole(role);
    handle->setHandleShape(shape);
    handle->setPos(pos);

    addAbstractHandle(handle);

    return handle;
}

GraphicsBezierHandle *SchItem::addBezierHandle(int id, const QPointF &pos)
{
    Q_UNUSED(pos);
    GraphicsBezierHandle *handle = new GraphicsBezierHandle(this);

    Q_ASSERT(!m_idToHandle.keys().contains(id));

    handle->setBehaviour(NormalHandleBehaviour);
    handle->setParentGraphicsObject(this);
    handle->setHandleId(id);
    //handle->setPos(pos);

    addAbstractHandle(handle);

    return handle;
}

void SchItem::removeHandle(int id)
{
    Q_ASSERT(m_idToHandle.keys().contains(id));

    removeHandle(m_idToHandle[id]);
}

void SchItem::removeHandle(AbstractGraphicsHandle *handle)
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

void SchItem::removeAllHandles()
{
    blockItemNotification();
    foreach (AbstractGraphicsHandle *handle, m_handleToId.keys()) {
        removeObservedItem(handle);
        handle->setParentItem(nullptr);
        delete handle;
    }
    unblockItemNotification();
    m_handleToId.clear();
    m_idToHandle.clear();
}

GraphicsRegularHandle *SchItem::regularHandleAt(int id) const
{
    Q_ASSERT(id < handleCount());
    return static_cast<GraphicsRegularHandle *>(m_idToHandle[id]);
}

GraphicsBezierHandle *SchItem::bezierHandleAt(int id) const
{
    Q_ASSERT(id < handleCount());
    return static_cast<GraphicsBezierHandle *>(m_idToHandle[id]);
}
