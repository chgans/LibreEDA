#include "placepolylinecommand.h"

namespace SymbolEditor
{

    PlacePolylineCommand::PlacePolylineCommand(UndoCommand *parent):
        PlacementCommand(parent)
    {
        setText("Place polyline");
    }

    void PlacePolylineCommand::undo()
    {

    }

    void PlacePolylineCommand::redo()
    {

    }

}
