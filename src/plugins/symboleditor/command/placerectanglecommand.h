#pragma once

#include "placementcommand.h"

namespace SymbolEditor
{

    class PlaceRectangleCommand: public PlacementCommand
    {
    public:
        PlaceRectangleCommand(UndoCommand *parent = nullptr);

        void undo();
        void redo();

        QPointF topLeft;
        QPointF bottomRight;
    };

}
