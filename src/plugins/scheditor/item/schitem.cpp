#include "item/schitem.h"
#include "handle/abstractgraphicshandle.h"
#include "handle/graphicsregularhandle.h"
#include "handle/graphicsbezierhandle.h"
#include "core/json.h"

#include <QGraphicsTransform>

const QString SchItem::J_POSITION = QStringLiteral("position");
const QString SchItem::J_ROTATION = QStringLiteral("rotation");
const QString SchItem::J_ZVALUE = QStringLiteral("z-value");
const QString SchItem::J_LOCKED = QStringLiteral("locked");
const QString SchItem::J_PEN = QStringLiteral("pen");
const QString SchItem::J_BRUSH = QStringLiteral("brush");

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
    dst->setTransform(transform());
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

void SchItem::updateMirroringTransform()
{
    qreal m11 = m_isXMirrored ? -1 : 1;
    qreal m22 = m_isYMirrored ? -1 : 1;
    setTransform(QTransform(m11, 0,   0,
                            0,   m22, 0,
                            0,   0,   1));
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

bool SchItem::isXMirrored() const
{
    return m_isXMirrored;
}

bool SchItem::isYMirrored() const
{
    return m_isYMirrored;
}

bool SchItem::fromJson(QString *errorString, const QJsonObject &jsonObject)
{
    QPointF pos;
    if (!Json::toPoint(errorString, jsonObject.value(J_POSITION), pos))
        return false;
    qreal rot = 0;
    if (jsonObject.contains(J_ROTATION) && !Json::toReal(errorString, jsonObject.value(J_ROTATION), rot))
        return false;
    QPen pen;
    if (!Json::toPen(errorString, jsonObject.value(J_PEN), pen))
        return false;
    QBrush brush;
    if (!Json::toBrush(errorString, jsonObject.value(J_BRUSH), brush))
        return false;
    setPos(pos);
    setRotation(rot);
    setPen(pen);
    setBrush(brush);
    return true;
}

void SchItem::toJson(QJsonObject &jsonObject) const
{
    jsonObject.insert(J_POSITION, Json::fromPoint(pos()));
    jsonObject.insert(J_ROTATION, QJsonValue(rotation()));
}

// Return a list of hot spots in item's coordinate
QList<QPointF> SchItem::hotSpots() const
{
    QList<QPointF> points;
    foreach (AbstractGraphicsHandle *handle, m_handleToId.keys()) {
        Q_ASSERT(handle->parentItem() == this);
        points.append(handle->pos());
    }
    return points;
}

QList<QPointF> SchItem::endPoints() const
{
    return QList<QPointF>();
}

QList<QPointF> SchItem::midPoints() const
{
    return QList<QPointF>();
}

QList<QPointF> SchItem::centerPoints() const
{
    return QList<QPointF>();
}

QList<QPointF> SchItem::nearestPoints(QPointF pos) const
{
    Q_UNUSED(pos);
    return QList<QPointF>();
}

QList<QLineF> SchItem::axes() const
{
    QList<QLineF> axes;
    for (qreal angle = 0; angle < 360.0; angle+= 45)
        axes << QLineF::fromPolar(UINT_MAX, angle).translated(pos());
    return axes;
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

void SchItem::setXMirrored(bool mirrored)
{
    if (m_isXMirrored == mirrored)
        return;
    m_isXMirrored = mirrored;
    updateMirroringTransform();
    emit xMirroredChanged();
}

void SchItem::setYMirrored(bool mirrored)
{
    if (m_isYMirrored == mirrored)
        return;
    m_isYMirrored = mirrored;
    updateMirroringTransform();
    emit yMirroredChanged();
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
