#pragma once

#include "command.h"
#include "xdl/symbol.h"

#include <QUndoCommand>
#include <QPen>
#include <QPointF>
#include <QFont>


namespace SymbolEditor
{

    class PlacementCommand: public UndoCommand
    {
    public:
        PlacementCommand(UndoCommand *parent = nullptr);

    protected:
        void removeItem();
        void placeItem(xdl::symbol::Item *item);

    public:
        quint64 itemId;

        // 1 - Visual
        xdl::symbol::LineStyle lineStyle;
        xdl::symbol::LineWidth lineWidth;
        xdl::symbol::Color lineColor;
        xdl::symbol::FillStyle fillStyle;
        xdl::symbol::Color fillColor;
        qreal opacity;
        // 2 - Geometry
        QPointF position;
        qreal rotation;
        bool xMirrored;
        bool yMirrored;
        // 3 - State
        bool locked;
        bool visible;
        qreal zValue;
    };

}
