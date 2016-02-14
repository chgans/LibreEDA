#ifndef GRAPHICSARCTOOL_H
#define GRAPHICSARCTOOL_H

#include "tool/abstractgraphicsinserttool.h"

class GraphicsArcItem;
class PenSettingsWidget;
class BrushSettingsWidget;

class QAction;

class GraphicsArcTool : public AbstractGraphicsInsertTool
{
    Q_OBJECT

public:
    GraphicsArcTool(QObject *parent = nullptr);
    ~GraphicsArcTool();

private:
    GraphicsArcItem *m_item;
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

#endif // GRAPHICSARCTOOL_H
