#ifndef GRAPHICSPOLYGONTOOL_H
#define GRAPHICSPOLYGONTOOL_H

#include "tool/abstractgraphicsinserttool.h"

class GraphicsPolygonItem;
class PenSettingsWidget;
class BrushSettingsWidget;

class QAction;

class GraphicsPolygonTool : public AbstractGraphicsInsertTool
{
    Q_OBJECT

public:
    GraphicsPolygonTool(QObject *parent = 0);
    ~GraphicsPolygonTool();

private:
    GraphicsPolygonItem *m_item;
    PenSettingsWidget *m_penSettingsWidget;
    BrushSettingsWidget *m_brushSettingsWidget;

    // GraphicsTool interface
public:
    virtual void activate(const QAction *which);
    virtual void desactivate(const QAction *which);

    // AbstractGraphicsInsertTool interface
public:
    virtual SchItem *beginInsert(const QPointF &pos);
    virtual void addPoint(int idx, const QPointF &pos);
    virtual void freezePoint(int idx, const QPointF &pos);
    virtual bool removePoint(int idx, const QPointF &pos);
    virtual void movePoint(int idx, const QPointF &pos);
    virtual void endInsert(const QPointF &pos);
    virtual void cancelInsert();
};

#endif // GRAPHICSPOLYGONTOOL_H
