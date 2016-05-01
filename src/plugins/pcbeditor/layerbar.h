#ifndef LAYERBAR_H
#define LAYERBAR_H

#include <QTabBar>
#include <QList>
#include <QMap>

#include <functional>

class DesignLayer;
class QMenu;

class LayerTabBar : public QTabBar
{
    Q_OBJECT
public:
    typedef std::function<void(QMenu*)> MenuProvider;

    explicit LayerTabBar(QWidget *parent = nullptr);
    ~LayerTabBar();

    DesignLayer *currentLayer() const;

public slots:
    void addLayerTab(DesignLayer *layer, bool visible = true);

    void removeLayerTab(DesignLayer *layer);
    //void replaceLayerTabs(QList<DesignLayer *> layers);

    void hideLayerTab(DesignLayer *layer);
    void showLayerTab(DesignLayer *layer);
    //void hideLayerTabs(QList<DesignLayer *> layers);
    //void showLayerTabs(QList<DesignLayer *> layers);

    void setCurrentLayer(DesignLayer *layer);

signals:
    void currentLayerChanged(DesignLayer *layer);

    // TBD
    void hideLayerRequested(DesignLayer *layer);
    void showLayerRequested(DesignLayer *layer);

private slots:
    void onCurrentIndexChanged(int index);

private:
    QList<DesignLayer *> m_allLayers;
    QList<DesignLayer *> m_visibleLayers;
    QList<DesignLayer *> m_hiddenLayers;

    int layerToIndex(DesignLayer *layer);
    DesignLayer *indexToLayer(int index) const;
};

#endif // LAYERBAR_H
