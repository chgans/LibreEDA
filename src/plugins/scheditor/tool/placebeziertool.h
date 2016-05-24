#ifndef PLACEBEZIERTOOL_H
#define PLACEBEZIERTOOL_H

#include "tool/placementtool.h"

#include <QPointF>

class BezierItem;
class PenSettingsWidget;
class BrushSettingsWidget;

class PlaceBezierTool : public PlacementTool
{
    Q_OBJECT

public:
    explicit PlaceBezierTool(QObject *parent = nullptr);
    ~PlaceBezierTool();

private:
    BezierItem *m_item;
    PenSettingsWidget *m_penSettingsWidget;
    BrushSettingsWidget *m_brushSettingsWidget;

    // PlacementTool interface
public:
    Item *beginInsert(const QPointF &pos);
    void addPoint(int idx, const QPointF &pos);
    void freezePoint(int idx, const QPointF &pos);
    bool removePoint(int idx, const QPointF &pos);
    void movePoint(int idx, const QPointF &pos);
    void endInsert(const QPointF &pos);
    void cancelInsert();
};

#endif // PLACEBEZIERTOOL_H
