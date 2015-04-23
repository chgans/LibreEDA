#include "designlayer.h"
#include "items/graphicsitem.h"

/*
 * TODO:
 *  - DesignLayer is just a QObject
 *  - DesignLayer has a QGraphicsItem based layerItem
 *  - rename "present" to "enable"
 *  - Don't use any QGItem for layers as a PCB component is made of items on
 *    different layers and we'll certainly implement this a a QGItemGroup
 */

DesignLayer::DesignLayer(QGraphicsItem *parent):
    QGraphicsObject(parent),
    m_index(-1),
    m_category(InvalidCategory),
    m_face(InvalidFace),
    m_colorMode(NormalColorMode),
    m_pairedLayer(nullptr)
{
    setFlags(ItemHasNoContents);
    for (Primitive::Type type = Primitive::_BeginType; type < Primitive::_EndType; type = Primitive::Type(type + 1)) {
        m_primitiveOpacityMap[type] = 1.0;
    }
    setEnabled(false);
}

DesignLayer::~DesignLayer()
{

}

QString DesignLayer::defaultName() const
{
    return m_name;
}

QString DesignLayer::customName() const
{
    return m_customName;
}

QString DesignLayer::effectiveName() const
{
    if (m_customName.isEmpty())
        return m_name;
    return m_customName;
}

void DesignLayer::setDefaultName(const QString &name)
{
    if (m_name == name)
        return;

    m_name = name;

    emit defaultNameChanged(m_name);
}

void DesignLayer::setCustomName(const QString &name)
{
    if (m_customName == name)
        return;

    m_customName = name;

    emit customNameChanged(m_customName);
}

const QColor &DesignLayer::color() const
{
    return m_color;
}

void DesignLayer::setColor(const QColor &color)
{
    if (m_color == color)
        return;

    m_color = color;
    updateItems();

    emit colorChanged(m_color);
}

QRectF DesignLayer::boundingRect() const
{
    return QRectF();
}

void DesignLayer::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

DesignLayer *DesignLayer::pairedLayer() const
{
    return m_pairedLayer;
}

bool DesignLayer::isValid() const
{
    return !m_name.isEmpty() && m_color.isValid() && m_index != -1 &&
            m_category != InvalidCategory && m_face != InvalidFace;
}

bool DesignLayer::isUsed() const
{
    return childItems().count() != 0;
}

qreal DesignLayer::opacityForPrimitive(Primitive::Type type)
{
    Q_ASSERT(m_primitiveOpacityMap.contains(type));
    return m_primitiveOpacityMap[type];
}

void DesignLayer::setOpacityForPrimitive(Primitive::Type type, qreal opacity)
{
    Q_ASSERT(m_primitiveOpacityMap.contains(type));
    m_primitiveOpacityMap[type] = opacity;
}


DesignLayer::ColorMode DesignLayer::colorMode() const
{
    return m_colorMode;
}

QColor DesignLayer::effectiveColor() const
{
    switch(m_colorMode)
    {
    case NormalColorMode:
        return color();
    case MonochromedMode:
        return QColor(0x2F, 0x2F, 0x2F);
    case GrayscaledMode: {
        QColor newColor = color().toHsv();
        newColor.setHsv(newColor.hsvHue(), 0, newColor.value());
        return newColor;
    }
    case HiddenMode:
        return QColor();
    default:
        Q_ASSERT(false);
        return QColor();
    }
}

void DesignLayer::addItem(GraphicsItem *item)
{
    item->setParentItem(this);
    item->setLayer(this);
    m_items.append(item);
}

QList<GraphicsItem *> DesignLayer::items() const
{
    return m_items;
}

DesignLayer::Face DesignLayer::face() const
{
    return m_face;
}

int DesignLayer::index() const
{
    return m_index;
}

DesignLayer::Category DesignLayer::category() const
{
    return m_category;
}

void DesignLayer::setIndex(int position)
{
    if (m_index == position)
        return;

    m_index = position;
    emit indexChanged(position);
}

void DesignLayer::setCategory(DesignLayer::Category category)
{
    if (m_category == category)
        return;

    m_category = category;
    emit categoryChanged(category);
}

void DesignLayer::setFace(DesignLayer::Face arg)
{
    if (m_face == arg)
        return;

    m_face = arg;
    emit faceChanged(arg);
}

void DesignLayer::setPairedLayer(DesignLayer *layer)
{
    if (m_pairedLayer == layer)
        return;

    m_pairedLayer = layer;
    emit pairedLayerChanged(layer);
}

void DesignLayer::setPresent(bool arg)
{
    if (m_present == arg)
        return;

    m_present = arg;
    emit presentChanged(arg);
}

void DesignLayer::setColorMode(DesignLayer::ColorMode mode)
{
    if (m_colorMode == mode)
        return;

    m_colorMode = mode;
    updateItems();
    emit colorModeChanged(mode);
    emit effectiveColorChanged(effectiveColor());
}

void DesignLayer::updateItems()
{
    foreach(GraphicsItem *item, m_items)
        item->update();
}
