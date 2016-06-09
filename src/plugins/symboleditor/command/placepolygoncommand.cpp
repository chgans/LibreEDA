#include "placepolygoncommand.h"

namespace SymbolEditor
{

    PlacePolygonCommand::PlacePolygonCommand(UndoCommand *parent):
        PlacementCommand(parent)
    {
        setText("Place polygon");
    }

    void PlacePolygonCommand::undo()
    {
        removeItem();
    }

    void PlacePolygonCommand::redo()
    {
        auto polygon = new xdl::symbol::PolygonItem;
        polygon->setVertices(vertices);
        // Position should be [0, 0]
        placeItem(polygon);
    }

}
