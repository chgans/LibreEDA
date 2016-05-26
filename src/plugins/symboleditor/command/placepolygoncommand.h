#pragma once

#include "placementcommand.h"

namespace SymbolEditor
{

    class PlacePolygonCommand: public PlacementCommand
    {
    public:
        PlacePolygonCommand(UndoCommand *parent = nullptr);

        void undo();
        void redo();

        QList<QPointF> vertices; // position is always first point?
    };

}
