#include "placementcommand.h"
#include "document.h"

namespace SymbolEditor
{

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
        item->setLineStyle(lineStyle);
        item->setLineWidth(lineWidth);
        item->setLineColor(lineColor);
        item->setFillStyle(fillStyle);
        item->setFillColor(fillColor);
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

}
