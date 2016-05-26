#include "placecirclecommand.h"

using namespace SymbolEditor;

PlaceCircleCommand::PlaceCircleCommand(UndoCommand *parent):
    PlacementCommand(parent)
{
    setText("Place circle");
}

void PlaceCircleCommand::undo()
{
    removeItem();
}

void PlaceCircleCommand::redo()
{
    auto circle = new xdl::symbol::CircleItem;
    circle->center = center;
    circle->radius = radius;
    placeItem(circle);
}
