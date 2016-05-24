#ifndef PLACEELLIPSETOOL_H
#define PLACEELLIPSETOOL_H

#include "tool/placementtool.h"

class EllipseItem;
class PenSettingsWidget;
class BrushSettingsWidget;

class QAction;

class PlaceEllipseTool : public PlacementTool
{
    Q_OBJECT

public:
    explicit PlaceEllipseTool(QObject *parent = nullptr);
    ~PlaceEllipseTool();

private:
    EllipseItem *m_item;
    PenSettingsWidget *m_penSettingsWidget;
    BrushSettingsWidget *m_brushSettingsWidget;

    // PlacementTool interface
public:
    virtual Item *beginInsert(const QPointF &pos);
    virtual void addPoint(int idx, const QPointF &pos);
    virtual void freezePoint(int idx, const QPointF &pos);
    virtual bool removePoint(int idx, const QPointF &pos);
    virtual void movePoint(int idx, const QPointF &pos);
    virtual void endInsert(const QPointF &pos);
    virtual void cancelInsert();
};

#endif // PLACEELLIPSETOOL_H
