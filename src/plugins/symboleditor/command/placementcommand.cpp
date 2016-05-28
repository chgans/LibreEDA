#include "placementcommand.h"
#include "document.h"

using namespace SymbolEditor;

PlacementCommand::PlacementCommand(UndoCommand *parent):
    UndoCommand(parent)
{

}

void PlacementCommand::removeItem()
{
    document()->removeItem(itemId);
}

void PlacementCommand::placeItem(xdl::symbol::Item *item)
{
    item->pen = pen;
    item->brush = brush;
    item->position = position;
    item->rotation = rotation;
    item->opacity = opacity;
    item->zValue = zValue;
    item->locked = locked;
    item->visible = visible;
    item->xMirrored = xMirrored;
    item->yMirrored = yMirrored;
    itemId = document()->addItem(item);
}
