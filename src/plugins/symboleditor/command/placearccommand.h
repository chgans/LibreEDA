#pragma once

#include "placementcommand.h"

namespace SymbolEditor
{

    class PlaceCircularArcCommand: public PlacementCommand
    {
    public:
        PlaceCircularArcCommand(UndoCommand *parent = nullptr);

        void undo();
        void redo();

        QPointF center; // = position ?
        qreal radius;
        qreal startAngle;
        qreal spanAngle;
    };

}
