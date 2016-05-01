#ifndef GRAPHICSBEZIERTOOL_H
#define GRAPHICSBEZIERTOOL_H

#include "tool/abstractgraphicsinserttool.h"

#include <QPointF>

class GraphicsBezierItem;
class PenSettingsWidget;
class BrushSettingsWidget;

class GraphicsBezierTool : public AbstractGraphicsInsertTool
{
    Q_OBJECT

public:
    explicit GraphicsBezierTool(QObject *parent = nullptr);
    ~GraphicsBezierTool();

private:
    GraphicsBezierItem *m_item;
    PenSettingsWidget *m_penSettingsWidget;
    BrushSettingsWidget *m_brushSettingsWidget;

    // GraphicsTool interface
public:
    virtual void activate(const QAction *which);
    virtual void desactivate(const QAction *which);

    // AbstractGraphicsInsertTool interface
public:
    SchItem *beginInsert(const QPointF &pos);
    void addPoint(int idx, const QPointF &pos);
    void freezePoint(int idx, const QPointF &pos);
    bool removePoint(int idx, const QPointF &pos);
    void movePoint(int idx, const QPointF &pos);
    void endInsert(const QPointF &pos);
    void cancelInsert();
};

#endif // GRAPHICSBEZIERTOOL_H
