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
    item->setLineStyle(xdl::symbol::LineStyle(pen.style()));
    item->setLineColor(pen.color().name());
    item->setLineWidth(xdl::symbol::MediumLine); // FIXME
    item->setFillColor(brush.color().name());
    item->setPosition(position);
    item->setRotation(rotation);
    item->setOpacity(opacity);
    //item->zValue = zValue;
    item->setLocked(locked);
    item->setVisible(visible);
    item->setXMirrored(xMirrored);
    item->setYMirrored(yMirrored);
    itemId = document()->addItem(item);
}
