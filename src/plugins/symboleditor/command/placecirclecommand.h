#pragma once

#include "placementcommand.h"

namespace SymbolEditor
{

    class PlaceCircleCommand: public PlacementCommand
    {
    public:
        PlaceCircleCommand(UndoCommand *parent = nullptr);

        void undo();
        void redo();

        QPointF center; // = position ?
        qreal radius;
    };

}
