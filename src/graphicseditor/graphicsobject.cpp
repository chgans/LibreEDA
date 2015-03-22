#include "graphicsobject.h"
#include "graphicshandle.h"

#include <QDebug>

GraphicsObject::GraphicsObject(GraphicsObject *parent):
    QGraphicsObject(parent)
{
    setPen(QPen(QBrush(QColor::fromRgb(0x80, 0x00, 0x00)), 0.5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    setBrush(QBrush(QColor::fromRgb(0xff, 0xff, 0xb0)));
    setFlags(QGraphicsItem::ItemIsSelectable|QGraphicsItem::ItemIsMovable);
}

GraphicsObject::~GraphicsObject()
{
}

int GraphicsObject::handleCount() const
{
    return childItems().count();
}


void GraphicsObject::cloneTo(GraphicsObject *dst)
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

#if 0
    foreach (GraphicsHandle *other, m_handles) {
        // FIXME: dst.moveHandle(idx, pos);
        //dst->addHandle(other->clone(dst));
        Q_UNUSED(other);
        Q_UNUSED(dst);
    }
#endif
}

// From Qt qgraphicsitem.cpp: qt_graphicsItem_shapeFromPath()
QPainterPath GraphicsObject::shapeFromPath(const QPainterPath &path, const QPen &pen)
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
    p.addPath(path); // not needed for open path
    return p;
}

GraphicsHandle *GraphicsObject::handleAt(int idx)
{
    Q_ASSERT(idx < childItems().count());
    return dynamic_cast<GraphicsHandle*>(childItems().at(idx));
}


QPen GraphicsObject::pen() const
{
    return m_pen;
}

QBrush GraphicsObject::brush() const
{
    return m_brush;
}

void GraphicsObject::setPen(const QPen &pen)
{
    if (m_pen == pen)
        return;

    m_pen = pen;
    prepareGeometryChange();
    update();
    emit penChanged(pen);
}

void GraphicsObject::setBrush(const QBrush &brush)
{
    if (m_brush == brush)
        return;

    m_brush = brush;
    update();
    emit brushChanged(brush);
}
