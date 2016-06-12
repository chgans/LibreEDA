#include "item/polygonitem.h"

namespace SymbolEditor
{

    PolygonItem::PolygonItem(Item *parent):
        Item(parent),
        m_fillRule(Qt::OddEvenFill)
    {

    }

    PolygonItem::~PolygonItem()
    {

    }

    Qt::FillRule PolygonItem::fillRule() const
    {
        return m_fillRule;
    }

    QPolygonF PolygonItem::polygon() const
    {
        return m_polygon;
    }

    void PolygonItem::setFillRule(Qt::FillRule fillRule)
    {
        if (m_fillRule == fillRule)
        {
            return;
        }

        m_fillRule = fillRule;
        update();
    }

    void PolygonItem::setPolygon(QPolygonF polygon)
    {
        if (m_polygon == polygon)
        {
            return;
        }

        removeAllHandles();

        prepareGeometryChange();
        m_polygon = polygon;
        for (int i = 0; i < polygon.count(); i++)
        {
            addRegularHandle(i, MoveHandleRole, CircularHandleShape, polygon[i]);
        }
        m_boundingRect = QRectF();
        update();
    }

    void PolygonItem::addPoint(const QPointF &pos)
    {
        addRegularHandle(m_polygon.count(), MoveHandleRole, CircularHandleShape, pos);
        m_polygon.append(pos);
        handleToPolygon();
    }

    void PolygonItem::movePoint(int idx, const QPointF &pos)
    {
        blockItemNotification();
        m_idToHandle[idx]->setPos(pos);
        unblockItemNotification();
        handleToPolygon();
    }

    void PolygonItem::handleToPolygon()
    {
        prepareGeometryChange();
        m_polygon = QPolygonF();
        for (int i = 0; i < m_idToHandle.count(); i++)
        {
            m_polygon.append(m_idToHandle[i]->pos());
        }
        m_boundingRect = QRectF();
        update();
    }

    void PolygonItem::polygonToHandle()
    {
        blockItemNotification();
        for (int i = 0; i < m_polygon.count(); i++)
        {
            m_idToHandle[i]->setPos(m_polygon[i].x(),
                                    m_polygon[i].y());
        }
        unblockItemNotification();
    }

    QRectF PolygonItem::boundingRect() const
    {
        if (m_boundingRect.isNull())
        {
            qreal pw = pen().style() == Qt::NoPen ? qreal(0) : pen().widthF();
            if (pw == 0.0)
            {
                m_boundingRect = m_polygon.boundingRect();
            }
            else
            {
                m_boundingRect = shape().controlPointRect();
            }
        }
        return m_boundingRect;
    }

    QPainterPath PolygonItem::shape() const
    {
        QPainterPath path;
        path.addPolygon(polygon());
        return shapeFromPath(path, pen());
    }

    void PolygonItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                            QWidget *widget)
    {
        Q_UNUSED(option);
        Q_UNUSED(widget);
        painter->setPen(pen());
        painter->setBrush(brush());
        painter->drawPolygon(polygon(), fillRule());
    }

    Item *PolygonItem::clone()
    {
        PolygonItem *item = new PolygonItem();
        item->setPolygon(polygon());
        item->setFillRule(fillRule());
        Item::cloneTo(item);
        return item;
    }

    void PolygonItem::setProperty(quint64 id, const QVariant &value)
    {
        Item::setProperty(id, value);
    }

    QVariant PolygonItem::itemChange(GraphicsItemChange change, const QVariant &value)
    {
        if (change == QGraphicsItem::ItemSelectedHasChanged)
        {
            for (Handle *handle : m_idToHandle)
            {
                handle->setVisible(isSelected());
            }
        }
        return value;
    }

    void PolygonItem::itemNotification(IObservableItem *item)
    {
        Q_UNUSED(item);
        handleToPolygon();
    }

}
