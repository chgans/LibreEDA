#include "graphicsitem.h"

#include "designlayer.h"

GraphicsItem::GraphicsItem(GraphicsItem *parent):
    QGraphicsItem(parent)
{
    setLocked(false);
}

GraphicsItem::~GraphicsItem()
{

}

DesignLayer *GraphicsItem::layer() const
{
    return m_layer;
}

void GraphicsItem::setLayer(DesignLayer *layer)
{
    if (m_layer == layer)
        return;
    m_layer = layer;
    prepareGeometryChange();
}

bool GraphicsItem::isLocked() const
{
    return !flags().testFlag(QGraphicsItem::ItemIsMovable) &&
            !flags().testFlag(QGraphicsItem::ItemIsSelectable);
}

void GraphicsItem::setLocked(bool locked)
{
    if (locked == isLocked())
        return;
    setFlag(QGraphicsItem::ItemIsMovable, !locked);
    setFlag(QGraphicsItem::ItemIsSelectable, !locked);
}

bool GraphicsItem::shouldPaint() const
{
    return color().isValid();
}

QColor GraphicsItem::color() const
{
    Q_ASSERT(m_layer);
    return m_layer->effectiveColor();
}

