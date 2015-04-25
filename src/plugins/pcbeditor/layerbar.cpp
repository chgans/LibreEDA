#include "layerbar.h"
#include "designlayer.h"

#include <QIcon>
#include <QPainter>
#include <QPalette>
#include <QDebug>

static QIcon createColorIcon(const QColor &color)
{
    int size = 16;
    QPixmap pix(size, size);
    QPainter painter(&pix);
    QPalette palette;
    painter.setPen(palette.color(QPalette::Shadow));
    painter.setBrush(color);
    size--;
    painter.drawRect(0, 0, size, size);
    return QIcon(pix);
}

LayerTabBar::LayerTabBar(QWidget *parent):
    QTabBar(parent)
{
    setMovable(false);
    setTabsClosable(false);
    connect(this, &QTabBar::currentChanged,
            this, &LayerTabBar::onCurrentIndexChanged);
}

LayerTabBar::~LayerTabBar()
{

}

DesignLayer *LayerTabBar::currentLayer() const
{
    return indexToLayer(currentIndex());
}

void LayerTabBar::addLayerTab(DesignLayer *layer, bool visible)
{
    if (m_allLayers.contains(layer)) {
        qWarning() << __PRETTY_FUNCTION__ << "Layer" << layer->defaultName() << "already added";
        return;
    }
    if (visible) {
        int layerTabIndex = -1;
        // Keep layer tab in layer index (design layer stack position)
        for (int index = 0; index < count() ; index++) {
            DesignLayer *current = indexToLayer(index);
            if (current->index() > layer->index()) {
                layerTabIndex = index;
                break;
            }
        }
        QIcon icon = createColorIcon(layer->color());
        QString text = layer->effectiveName();
        blockSignals(true);
        if (layerTabIndex == -1)
            layerTabIndex = addTab(icon, text);
        else
            insertTab(layerTabIndex, layer->effectiveName());
        setTabData(layerTabIndex, QVariant::fromValue<DesignLayer *>(layer));
        blockSignals(false);
        m_visibleLayers.append(layer);
    }
    else {
        m_hiddenLayers.append(layer);
    }
    m_allLayers.append(layer);
}

void LayerTabBar::removeLayerTab(DesignLayer *layer)
{
    if (!m_allLayers.contains(layer)) {
        qWarning() << __PRETTY_FUNCTION__ << "Layer" << layer->defaultName() << "not found";
        return;
    }

    if (m_visibleLayers.contains(layer)) {
        removeTab(layerToIndex(layer));
        m_visibleLayers.removeOne(layer);
    }
    else if (m_hiddenLayers.contains(layer)) {
        m_hiddenLayers.removeOne(layer);
    }
    m_allLayers.removeOne(layer);
}

void LayerTabBar::hideLayerTab(DesignLayer *layer)
{
    if (!m_visibleLayers.contains(layer)) {
        qWarning() << __PRETTY_FUNCTION__ << "Layer" << layer->defaultName() << "not found";
        return;
    }
    removeTab(layerToIndex(layer));
    m_visibleLayers.removeOne(layer);
    m_hiddenLayers.append(layer);
}

void LayerTabBar::showLayerTab(DesignLayer *layer)
{
    if (!m_hiddenLayers.contains(layer)) {
        qWarning() << __PRETTY_FUNCTION__ << "Layer" << layer->defaultName() << "not found";
        return;
    }
    addLayerTab(layer, true);
}

void LayerTabBar::setCurrentLayer(DesignLayer *layer)
{
    if (currentIndex() >= 0) {
        if (layer == indexToLayer(currentIndex()))
            return;
    }
    setCurrentIndex(layerToIndex(layer));
}

void LayerTabBar::onCurrentIndexChanged(int index)
{
    emit currentLayerChanged(indexToLayer(index));
}

int LayerTabBar::layerToIndex(DesignLayer *layer)
{
    for (int index = 0; index < count() ; index++) {
        if (indexToLayer(index) == layer)
            return index;
    }
    return -1;
}

DesignLayer *LayerTabBar::indexToLayer(int index) const
{
    if (index < 0 || index >= count()) {
        qWarning() << __PRETTY_FUNCTION__ << "Index out of range" << index;
        return nullptr;
    }
    return tabData(index).value<DesignLayer *>();
}
