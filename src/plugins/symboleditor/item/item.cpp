#include "item/item.h"
#include "handle/handle.h"
#include "handle/regularhandle.h"
#include "handle/bezierhandle.h"
#include "view/palette.h"
#include "xdl/symbol.h"

#include <QGraphicsTransform>

namespace SymbolEditor
{

    Item::Item(Item *parent):
        QGraphicsItem(parent)
    {
        setLineStyle(xdl::symbol::SolidLine);
        setLineWidth(xdl::symbol::MediumLine);
        setLineColor(xdl::symbol::PrimaryContent);
        setFillStyle(xdl::symbol::SolidFill);
        setFillColor(xdl::symbol::Background);
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
        dst->setLineStyle(lineStyle());
        dst->setLineWidth(lineWidth());
        dst->setLineColor(lineColor());
        dst->setFillStyle(fillStyle());
        dst->setFillColor(fillColor());
        dst->setEnabled(isEnabled());
        dst->setFlags(flags());
        dst->setOpacity(opacity());
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

    void Item::setLineStyle(Item::LineStyle style)
    {
        if (m_lineStyle == style)
        {
            return;
        }

        m_lineStyle = style;
        update();
    }

    Item::LineStyle Item::lineStyle() const
    {
        return m_lineStyle;
    }

    void Item::setLineWidth(Item::LineWidth width)
    {
        if (m_lineWidth == width)
        {
            return;
        }

        prepareGeometryChange();
        m_lineWidth = width;
        m_boundingRect = QRectF();
        update();
    }

    Item::LineWidth Item::lineWidth() const
    {
        return m_lineWidth;
    }

    void Item::setLineColor(Item::Color color)
    {
        if (m_lineColor == color)
        {
            return;
        }

        m_lineColor = color;
        update();
    }

    Item::Color Item::lineColor() const
    {
        return m_lineColor;
    }

    void Item::setFillStyle(Item::FillStyle style)
    {
        if (m_fillStyle == style)
        {
            return;
        }

        m_fillStyle = style;
        update();
    }

    Item::FillStyle Item::fillStyle() const
    {
        return m_fillStyle;
    }

    void Item::setFillColor(Item::Color color)
    {
        if (m_fillColor == color)
        {
            return;
        }

        m_fillColor = color;
        update();
    }

    Item::Color Item::fillColor() const
    {
        return m_fillColor;
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

    void Item::setProperty(quint64 id, const QVariant &value)
    {
        switch (id)
        {
            case xdl::symbol::Item::PositionProperty:
                setPos(value.toPointF());
                break;
            case xdl::symbol::Item::XMirroredProperty:
                setXMirrored(value.toBool());
                break;
            case xdl::symbol::Item::YMirroredProperty:
                setYMirrored(value.toBool());
                break;
            case xdl::symbol::Item::LockedProperty:
                setEnabled(!value.toBool());
                break;
            case xdl::symbol::Item::VisibilityProperty:
                setVisible(value.toBool());
                break;
            case xdl::symbol::Item::LineStyleProperty:
                setLineStyle(LineStyle(value.toInt()));
                break;
            case xdl::symbol::Item::LineWidthProperty:
                setLineWidth(LineWidth(value.toInt()));
                break;
            case xdl::symbol::Item::LineColorProperty:
                setLineColor(Color(value.toInt()));
                break;
            case xdl::symbol::Item::FillStyleProperty:
                setFillStyle(FillStyle(value.toInt()));
                break;
            case xdl::symbol::Item::FillColorProperty:
                setFillColor(Color(value.toInt()));
                break;
            default:
                break;
        }
    }

    QPen Item::pen() const
    {
        QPen pen;
        switch (m_lineStyle)
        {
            case xdl::symbol::NoLine:
                pen.setStyle(Qt::NoPen);
                break;
            case xdl::symbol::SolidLine:
                pen.setStyle(Qt::SolidLine);
                break;
            case xdl::symbol::DotLine:
                pen.setStyle(Qt::DotLine);
                break;
            case xdl::symbol::DashLine:
                pen.setStyle(Qt::DashLine);
                break;
            case xdl::symbol::DashDotLine:
                pen.setStyle(Qt::DashDotLine);
                break;
            case xdl::symbol::DashDotDotLine:
                pen.setStyle(Qt::DashDotDotLine);
                break;
        }
        switch (m_lineWidth)
        {
            case xdl::symbol::ThinestLine:
                pen.setWidthF(0.13);
                break;
            case xdl::symbol::ThinerLine:
                pen.setWidthF(0.18);
                break;
            case xdl::symbol::ThinLine:
                pen.setWidthF(0.25);
                break;
            case xdl::symbol::SlightlyThinLine:
                pen.setWidthF(0.35);
                break;
            case xdl::symbol::MediumLine:
                pen.setWidthF(0.5);
                break;
            case xdl::symbol::SlightlyThickLine:
                pen.setWidthF(0.7);
                break;
            case xdl::symbol::ThickLine:
                pen.setWidthF(1.0);
                break;
            case xdl::symbol::ThickerLine:
                pen.setWidthF(1.4);
                break;
            case xdl::symbol::ThickestLine:
                pen.setWidthF(2.0);
                break;
        }
        Palette palette;
        pen.setColor(palette.color(Palette::ColorId(m_lineColor)));
        return pen;
    }

    QBrush Item::brush() const
    {
        QBrush brush;
        switch (m_fillStyle)
        {
            case xdl::symbol::NoFill:
                brush.setStyle(Qt::NoBrush);
                break;
            case xdl::symbol::SolidFill:
                brush.setStyle(Qt::SolidPattern);
                break;
        }
        Palette palette;
        brush.setColor(palette.color(Palette::ColorId(m_fillColor)));
        return brush;
    }

    void Item::setXMirrored(bool mirrored)
    {
        if (m_isXMirrored == mirrored)
        {
            return;
        }
        m_isXMirrored = mirrored;
        updateMirroringTransform();
    }

    void Item::setYMirrored(bool mirrored)
    {
        if (m_isYMirrored == mirrored)
        {
            return;
        }
        m_isYMirrored = mirrored;
        updateMirroringTransform();
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

}
