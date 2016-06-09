#include "placerectanglecommand.h"

namespace SymbolEditor
{

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
        rectangle->setWidth(width);
        rectangle->setHeight(height);
        placeItem(rectangle);
    }

}
