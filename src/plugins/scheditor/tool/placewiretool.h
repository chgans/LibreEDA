#ifndef PLACEWIRETOOL_H
#define PLACEWIRETOOL_H

#include "tool/placementtool.h"

class PenSettingsWidget;
class WireOperationWidget;
class GraphicsWireItem;
class QAction;
class QLineEdit;

class PlaceWireTool : public PlacementTool
{
    Q_OBJECT

public:
    explicit PlaceWireTool(QObject *parent = nullptr);
    ~PlaceWireTool();

private:
    GraphicsWireItem *m_item;
    PenSettingsWidget *m_penSettingsWidget;

    // GraphicsTool interface
public:
    virtual void activate(const QAction *which);
    virtual void desactivate(const QAction *which);

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

#endif // PLACEWIRETOOL_H
