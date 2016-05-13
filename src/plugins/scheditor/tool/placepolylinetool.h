#ifndef PLACEPOLYLINETOOL_H
#define PLACEPOLYLINETOOL_H

#include "tool/placementtool.h"

class GraphicsLineItem;
class PenSettingsWidget;

class PlacePolyineTool : public PlacementTool
{
    Q_OBJECT

public:
    explicit PlacePolyineTool(QObject *parent = nullptr);

private:
    GraphicsLineItem *m_item;
    PenSettingsWidget *m_penSettingsWidget;

private:
    void setP1(const QPointF &pos);
    void setP2(const QPointF &pos);

    // GraphicsTool interface
public slots:
    virtual void cancel();

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

#endif // PLACEPOLYLINETOOL_H
