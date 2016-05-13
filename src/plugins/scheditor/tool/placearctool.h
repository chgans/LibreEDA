#ifndef PLACEARCTOOL_H
#define PLACEARCTOOL_H

#include "tool/placementtool.h"

class GraphicsArcItem;
class PenSettingsWidget;
class BrushSettingsWidget;

class QAction;

class PlaceArcTool : public PlacementTool
{
    Q_OBJECT

public:
    explicit PlaceArcTool(QObject *parent = nullptr);
    ~PlaceArcTool();

private:
    GraphicsArcItem *m_item;
    PenSettingsWidget *m_penSettingsWidget;
    BrushSettingsWidget *m_brushSettingsWidget;

    // PlacementTool interface
public:
    virtual SchItem *beginInsert(const QPointF &pos);
    virtual void addPoint(int idx, const QPointF &pos);
    virtual void freezePoint(int idx, const QPointF &pos);
    virtual bool removePoint(int idx, const QPointF &pos);
    virtual void movePoint(int idx, const QPointF &pos);
    virtual void endInsert(const QPointF &pos);
    virtual void cancelInsert();
};

#endif // PLACEARCTOOL_H
