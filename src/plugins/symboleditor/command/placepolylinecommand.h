#pragma once

#include "placementcommand.h"

namespace SymbolEditor
{

    class PlacePolylineCommand: public PlacementCommand
    {
    public:
        PlacePolylineCommand(UndoCommand *parent = nullptr);

        void undo();
        void redo();

        QList<QPointF> vertices; // position is always first point?
    };

}
