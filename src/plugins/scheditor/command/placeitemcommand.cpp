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
    removeItem();
}

void PlaceCircleCommand::redo()
{
    auto circle = new xdl::symbol::CircleItem;
    circle->center = center;
    circle->radius = radius;
    placeItem(circle);
}

PlaceCircularArcCommand::PlaceCircularArcCommand(UndoCommand *parent):
    PlacementCommand (parent)
{
    setText("Place circular arc");
}

void PlaceCircularArcCommand::undo()
{

}

void PlaceCircularArcCommand::redo()
{

}

PlaceEllipseCommand::PlaceEllipseCommand(UndoCommand *parent):
    PlacementCommand (parent)
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
    removeItem();
}

void PlacePolygonCommand::redo()
{
    auto polygon = new xdl::symbol::PolygonItem;
    polygon->vertices = vertices;
    placeItem(polygon);
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

MoveCommand::MoveCommand(UndoCommand *parent):
    UndoCommand (parent)
{
    setText("Move ? item(s)");
}

void MoveCommand::undo()
{
    for (quint64 id: itemIds)
    {
        auto item = document()->drawingItem(id);
        if (item == nullptr)
        {
            continue;
        }
        item->position -= delta;
        document()->updateDrawingItem(id);
    }
}

void MoveCommand::redo()
{
    for (quint64 id: itemIds)
    {
        auto item = document()->drawingItem(id);
        if (item == nullptr)
        {
            continue;
        }
        item->position += delta;
        document()->updateDrawingItem(id);
    }
    setText(QString("Move %1 item(s)").arg(itemIds.count()));
}
