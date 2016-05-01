#ifndef GRAPHICSELLIPSETOOL_H
#define GRAPHICSELLIPSETOOL_H

#include "tool/abstractgraphicsinserttool.h"

class GraphicsEllipseItem;
class PenSettingsWidget;
class BrushSettingsWidget;

class QAction;

class GraphicsEllipseTool : public AbstractGraphicsInsertTool
{
    Q_OBJECT

public:
    explicit GraphicsEllipseTool(QObject *parent = nullptr);
    ~GraphicsEllipseTool();

private:
    GraphicsEllipseItem *m_item;
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

#endif // GRAPHICSELLIPSETOOL_H
