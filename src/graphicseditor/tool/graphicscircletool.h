#ifndef GRAPHICSCIRCLETOOL_H
#define GRAPHICSCIRCLETOOL_H

#include "abstractgraphicsinserttool.h"

class GraphicsCircleItem;

class GraphicsCircleTool : public AbstractGraphicsInsertTool
{
public:
    GraphicsCircleTool(QObject *parent = 0);
    ~GraphicsCircleTool();

private:
    GraphicsCircleItem *m_item;

    // GraphicsTool interface
public:
    virtual void activate(const QAction *which);
    virtual void desactivate(const QAction *which);

    // AbstractGraphicsInsertTool interface
public:
    virtual GraphicsObject *beginInsert(const QPointF &pos);
    virtual void addPoint(int idx, const QPointF &pos);
    virtual void freezePoint(int idx, const QPointF &pos);
    virtual bool removePoint(int idx, const QPointF &pos);
    virtual void movePoint(int idx, const QPointF &pos);
    virtual void endInsert(const QPointF &pos);
    virtual void cancelInsert();
};

#endif // GRAPHICSCIRCLETOOL_H
