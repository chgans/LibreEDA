#ifndef PLACERECTANGLETOOL_H
#define PLACERECTANGLETOOL_H

#include "tool/placementtool.h"

#include <QRectF>

class PenSettingsWidget;
class BrushSettingsWidget;

namespace SymbolEditor
{

    class RectangleItem;

    class PlaceRectangleTool : public PlacementTool
    {
        Q_OBJECT

    public:
        explicit PlaceRectangleTool(QObject *parent = nullptr);
        ~PlaceRectangleTool();

    protected:
        void setP1(const QPointF &pos);
        void setP2(const QPointF &pos);

    private:
        RectangleItem *m_item;
        PenSettingsWidget *m_penSettingsWidget;
        BrushSettingsWidget *m_brushSettingsWidget;
        QRectF m_rect;

        // GraphicsTool interface
    public slots:
        virtual void cancel();

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

}

#endif // PLACERECTANGLETOOL_H
