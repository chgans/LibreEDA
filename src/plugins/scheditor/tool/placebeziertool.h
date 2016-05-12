#ifndef PLACEBEZIERTOOL_H
#define PLACEBEZIERTOOL_H

#include "tool/placementtool.h"

#include <QPointF>

class GraphicsBezierItem;
class PenSettingsWidget;
class BrushSettingsWidget;

class PlaceBezierTool : public PlacementTool
{
    Q_OBJECT

public:
    explicit PlaceBezierTool(QObject *parent = nullptr);
    ~PlaceBezierTool();

private:
    GraphicsBezierItem *m_item;
    PenSettingsWidget *m_penSettingsWidget;
    BrushSettingsWidget *m_brushSettingsWidget;

    // GraphicsTool interface
public:
    virtual void activate(const QAction *which);
    virtual void desactivate(const QAction *which);

    // PlacementTool interface
public:
    SchItem *beginInsert(const QPointF &pos);
    void addPoint(int idx, const QPointF &pos);
    void freezePoint(int idx, const QPointF &pos);
    bool removePoint(int idx, const QPointF &pos);
    void movePoint(int idx, const QPointF &pos);
    void endInsert(const QPointF &pos);
    void cancelInsert();
};

#endif // PLACEBEZIERTOOL_H
