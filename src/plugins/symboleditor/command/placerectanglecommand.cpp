#include "placerectanglecommand.h"

using namespace SymbolEditor;

PlaceRectangleCommand::PlaceRectangleCommand(UndoCommand *parent):
    PlacementCommand(parent)
{
    setText("Place rectangle");
}

void PlaceRectangleCommand::undo()
{
    removeItem();
}

void PlaceRectangleCommand::redo()
{
    auto rectangle = new xdl::symbol::RectangleItem;
    rectangle->topLeft = topLeft;
    rectangle->bottomRight = bottomRight;
    placeItem(rectangle);
}
