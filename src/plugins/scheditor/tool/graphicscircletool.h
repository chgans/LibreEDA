#ifndef GRAPHICSCIRCLETOOL_H
#define GRAPHICSCIRCLETOOL_H

#include "tool/abstractgraphicsinserttool.h"

class GraphicsCircleItem;
class PenSettingsWidget;
class BrushSettingsWidget;

class GraphicsCircleTool : public AbstractGraphicsInsertTool
{
    Q_OBJECT

public:
    explicit GraphicsCircleTool(QObject *parent = nullptr);
    ~GraphicsCircleTool();

private:
    GraphicsCircleItem *m_item;
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

#endif // GRAPHICSCIRCLETOOL_H
