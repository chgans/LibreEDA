#include "placearccommand.h"

namespace SymbolEditor
{

    PlaceCircularArcCommand::PlaceCircularArcCommand(UndoCommand *parent):
        PlacementCommand(parent)
    {
        setText("Place circular arc");
    }

    void PlaceCircularArcCommand::undo()
    {

    }

    void PlaceCircularArcCommand::redo()
    {

    }

}
