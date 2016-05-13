#include "placeitemcommand.h"
#include "scheditordocument.h"

UndoCommand::UndoCommand(UndoCommand *parent):
    QUndoCommand (parent), m_document(nullptr)
{

}

void UndoCommand::setDocument(SchEditorDocument *document)
{
    m_document = document;
}

SchEditorDocument *UndoCommand::document() const
{
    return m_document;
}

PlacementCommand::PlacementCommand(UndoCommand *parent):
    UndoCommand (parent)
{

}

void PlacementCommand::removeItem()
{
    document()->removeDrawingItem(itemId);
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
    itemId = document()->addDrawingItem(item);
}


PlaceRectangleCommand::PlaceRectangleCommand(UndoCommand *parent):
    PlacementCommand (parent)
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

PlaceCircleCommand::PlaceCircleCommand(UndoCommand *parent):
    PlacementCommand (parent)
{
    setText("Place circle");
}

void PlaceCircleCommand::undo()
{

}

void PlaceCircleCommand::redo()
{

}

PlaceCircularCommand::PlaceCircularCommand(UndoCommand *parent):
    PlacementCommand (parent)
{
    setText("Place circular arc");
}

void PlaceCircularCommand::undo()
{

}

void PlaceCircularCommand::redo()
{

}

PlaceEllipseCommand::PlaceEllipseCommand(UndoCommand *parent):
    PlacementCommand (parent)
{
    setText("Place ellipse");
}

void PlaceEllipseCommand::undo()
{

}

void PlaceEllipseCommand::redo()
{

}

PlaceEllipticalArcCommand::PlaceEllipticalArcCommand(UndoCommand *parent):
    PlacementCommand (parent)
{
    setText("Place elliptical arc");
}

void PlaceEllipticalArcCommand::undo()
{

}

void PlaceEllipticalArcCommand::redo()
{

}

PlacePolylineCommand::PlacePolylineCommand(UndoCommand *parent):
    PlacementCommand (parent)
{
    setText("Place polyline");
}

void PlacePolylineCommand::undo()
{

}

void PlacePolylineCommand::redo()
{

}

PlacePolygonCommand::PlacePolygonCommand(UndoCommand *parent):
    PlacementCommand (parent)
{
    setText("Place polygon");
}

void PlacePolygonCommand::undo()
{

}

void PlacePolygonCommand::redo()
{

}

PlaceLabelCommand::PlaceLabelCommand(UndoCommand *parent):
    PlacementCommand (parent)
{
    setText("Place label");
}

void PlaceLabelCommand::undo()
{

}

void PlaceLabelCommand::redo()
{

}
