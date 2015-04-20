#ifndef GRAPHICSITEM_H
#define GRAPHICSITEM_H

#include <QGraphicsItem>

class DesignLayer;

class GraphicsItem : public QGraphicsItem
{
public:
    GraphicsItem(GraphicsItem *parent = nullptr);
    ~GraphicsItem();

    DesignLayer *layer() const;
    void setLayer(DesignLayer *layer);

    bool isLocked() const;
    void setLocked(bool locked);

private:
    DesignLayer *m_layer;
};

#endif // GRAPHICSITEM_H
