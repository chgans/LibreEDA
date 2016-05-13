#ifndef PLACEPOLYGONTOOL_H
#define PLACEPOLYGONTOOL_H

#include "tool/placementtool.h"

class GraphicsPolygonItem;
class PenSettingsWidget;
class BrushSettingsWidget;

class QAction;

class PlacePolygonTool : public PlacementTool
{
    Q_OBJECT

public:
    explicit PlacePolygonTool(QObject *parent = nullptr);
    ~PlacePolygonTool();

private:
    GraphicsPolygonItem *m_item;
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

#endif // PLACEPOLYGONTOOL_H
