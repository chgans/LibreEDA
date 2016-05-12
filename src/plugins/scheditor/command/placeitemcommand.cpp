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

PlaceItemCommand::PlaceItemCommand(UndoCommand *parent):
    UndoCommand (parent)
{

}

void PlaceItemCommand::undo()
{
}

void PlaceItemCommand::redo()
{

}

ChangeItemVisual::ChangeItemVisual(UndoCommand *parent):
    UndoCommand (parent)
{

}

void ChangeItemVisual::undo()
{

}

void ChangeItemVisual::redo()
{

}

ChangeItemGeometry::ChangeItemGeometry(UndoCommand *parent):
    UndoCommand (parent)
{

}

void ChangeItemGeometry::undo()
{

}

void ChangeItemGeometry::redo()
{

}

ChangeItemState::ChangeItemState(UndoCommand *parent):
    UndoCommand (parent)
{

}

PlaceRectangleCommand::PlaceRectangleCommand(UndoCommand *parent):
    PlaceItemCommand (parent)
{

}

void PlaceRectangleCommand::undo()
{

}

void PlaceRectangleCommand::redo()
{

}

PlaceCircleCommand::PlaceCircleCommand(UndoCommand *parent):
    PlaceItemCommand (parent)
{

}

void PlaceCircleCommand::undo()
{

}

void PlaceCircleCommand::redo()
{

}

PlaceCircularCommand::PlaceCircularCommand(UndoCommand *parent):
    PlaceItemCommand (parent)
{

}

void PlaceCircularCommand::undo()
{

}

void PlaceCircularCommand::redo()
{

}

PlaceEllipseCommand::PlaceEllipseCommand(UndoCommand *parent):
    PlaceItemCommand (parent)
{

}

void PlaceEllipseCommand::undo()
{

}

void PlaceEllipseCommand::redo()
{

}

PlaceEllipticalArcCommand::PlaceEllipticalArcCommand(UndoCommand *parent):
    PlaceItemCommand (parent)
{

}

void PlaceEllipticalArcCommand::undo()
{

}

void PlaceEllipticalArcCommand::redo()
{

}

PlacePolylineCommand::PlacePolylineCommand(UndoCommand *parent):
    PlaceItemCommand (parent)
{

}

void PlacePolylineCommand::undo()
{

}

void PlacePolylineCommand::redo()
{

}

PlacePolygonCommand::PlacePolygonCommand(UndoCommand *parent):
    PlaceItemCommand (parent)
{

}

void PlacePolygonCommand::undo()
{

}

void PlacePolygonCommand::redo()
{

}

PlaceLabelCommand::PlaceLabelCommand(UndoCommand *parent):
    PlaceItemCommand (parent)
{

}

void PlaceLabelCommand::undo()
{

}

void PlaceLabelCommand::redo()
{

}
