#pragma once

#include "placementcommand.h"

namespace SymbolEditor
{

    class PlaceEllipseCommand: public PlacementCommand
    {
    public:
        PlaceEllipseCommand(UndoCommand *parent = nullptr);

        void undo();
        void redo();

        QPointF center; // = position ?
        qreal xRadius;
        qreal yRadius;
    };

}
