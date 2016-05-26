#include "placeellipsecommand.h"

using namespace SymbolEditor;

PlaceEllipseCommand::PlaceEllipseCommand(UndoCommand *parent):
    PlacementCommand(parent)
{
    setText("Place ellipse");
}

void PlaceEllipseCommand::undo()
{
    removeItem();
}

void PlaceEllipseCommand::redo()
{
    auto ellipse = new xdl::symbol::EllipseItem;
    ellipse->center = center;
    ellipse->xRadius = xRadius;
    ellipse->yRadius = yRadius;
    placeItem(ellipse);
}
