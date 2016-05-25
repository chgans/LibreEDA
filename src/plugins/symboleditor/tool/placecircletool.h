#pragma once

#include "tool/placementtool.h"

class PenSettingsWidget;
class BrushSettingsWidget;

namespace SymbolEditor
{

    class CircleItem;

    class PlaceCircleTool : public PlacementTool
    {
        Q_OBJECT

    public:
        explicit PlaceCircleTool(QObject *parent = nullptr);
        ~PlaceCircleTool();

    private:
        CircleItem *m_item;
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

}
