#include "placepolygoncommand.h"

using namespace SymbolEditor;

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
    polygon->vertices = vertices;
    placeItem(polygon);
}
