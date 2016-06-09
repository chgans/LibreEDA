#include "placecirclecommand.h"

namespace SymbolEditor
{

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
        //circle->setcenter(center);
        circle->setRadius(radius);
        placeItem(circle);
    }

}
