#include "placeitemcommand.h"
#include "scheditordocument.h"
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(LedaSymbolCommand)

Q_LOGGING_CATEGORY(LedaSymbolCommand, "leda.sch.command")

UndoCommand::UndoCommand(UndoCommand *parent):
    QUndoCommand(parent), m_document(nullptr)
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

void UndoCommand::warnItemNotFound(const QString command, quint64 id)
{
    qCWarning(LedaSymbolCommand) << QString("%1: Item '%2' not found").arg(command).arg(id);
}

PlacementCommand::PlacementCommand(UndoCommand *parent):
    UndoCommand(parent)
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
    circle->center = center;
    circle->radius = radius;
    placeItem(circle);
}

PlaceCircularArcCommand::PlaceCircularArcCommand(UndoCommand *parent):
    PlacementCommand(parent)
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

PlaceEllipticalArcCommand::PlaceEllipticalArcCommand(UndoCommand *parent):
    PlacementCommand(parent)
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
    PlacementCommand(parent)
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
    PlacementCommand(parent)
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
    PlacementCommand(parent)
{
    setText("Place label");
}

void PlaceLabelCommand::undo()
{

}

void PlaceLabelCommand::redo()
{

}

TranslateCommand::TranslateCommand(UndoCommand *parent):
    UndoCommand(parent)
{
    setText("Move ? item(s)");
}

void TranslateCommand::undo()
{
    for (quint64 id : itemIdList)
    {
        auto item = document()->drawingItem(id);
        if (item == nullptr)
        {
            warnItemNotFound("Translate", id);
            continue;
        }
        item->position -= amount;
        document()->updateDrawingItem(id);
    }
}

void TranslateCommand::redo()
{
    for (quint64 id : itemIdList)
    {
        auto item = document()->drawingItem(id);
        if (item == nullptr)
        {
            warnItemNotFound("Translate", id);
            continue;
        }
        item->position += amount;
        document()->updateDrawingItem(id);
    }

    setText(QString("Move %1 item").arg(itemIdList.count()));
    if (itemIdList.count() > 1)
    {
        setText(text() + "s");
    }
}

RotateCommand::RotateCommand(UndoCommand *parent):
    UndoCommand (parent)
{
    setText("Rotate item");
}

void RotateCommand::undo()
{
    for (quint64 id : itemIdList)
    {
        auto item = document()->drawingItem(id);
        if (item == nullptr)
        {
            warnItemNotFound("Rotate", id);
            continue;
        }
        item->rotation -= amount;
        document()->updateDrawingItem(id);
    }
}

void RotateCommand::redo()
{
    for (quint64 id : itemIdList)
    {
        auto item = document()->drawingItem(id);
        if (item == nullptr)
        {
            warnItemNotFound("Rotate", id);
            continue;
        }
        item->rotation += amount;
        document()->updateDrawingItem(id);
    }

    setText(QString("Rotate %1 item").arg(itemIdList.count()));
    if (itemIdList.count() > 1)
    {
        setText(text() + "s");
    }
}

MirrorCommand::MirrorCommand(UndoCommand *parent):
    UndoCommand (parent)
{

}

void MirrorCommand::undo()
{
    for (quint64 id : itemIdList)
    {
        auto item = document()->drawingItem(id);
        if (item == nullptr)
        {
            warnItemNotFound("Mirror", id);
            continue;
        }
        switch (orientation)
        {
            case Qt::Vertical:
            {
                item->yMirrored = !mirrored;
                break;
            }
            case Qt::Horizontal:
            {
                item->xMirrored = !mirrored;
                break;
            }
        }
        document()->updateDrawingItem(id);
    }
}

void MirrorCommand::redo()
{
    for (quint64 id : itemIdList)
    {
        auto item = document()->drawingItem(id);
        if (item == nullptr)
        {
            warnItemNotFound("Mirror", id);
            continue;
        }
        switch (orientation)
        {
            case Qt::Vertical:
            {
                item->yMirrored = mirrored;
                break;
            }
            case Qt::Horizontal:
            {
                item->xMirrored = mirrored;
                break;
            }
        }
        document()->updateDrawingItem(id);
    }

    setText(QString("Mirror %1 item").arg(itemIdList.count()));
    if (itemIdList.count() > 1)
    {
        setText(text() + "s");
    }
}

SetLockStateCommand::SetLockStateCommand(UndoCommand *parent):
    UndoCommand (parent)
{

}

void SetLockStateCommand::undo()
{
    for (quint64 id : itemIdList)
    {
        auto item = document()->drawingItem(id);
        if (item == nullptr)
        {
            warnItemNotFound("Lock", id);
            continue;
        }
        item->locked = !lockState;
        document()->updateDrawingItem(id);
    }
}

void SetLockStateCommand::redo()
{
    for (quint64 id : itemIdList)
    {
        auto item = document()->drawingItem(id);
        if (item == nullptr)
        {
            warnItemNotFound("Lock", id);
            continue;
        }
        item->locked = lockState;
        document()->updateDrawingItem(id);
    }

    if (lockState)
    {
        setText(QString("Lock %1 item").arg(itemIdList.count()));
    }
    else
    {
        setText(QString("Unlock %1 item").arg(itemIdList.count()));
    }

    if (itemIdList.count() > 1)
    {
        setText(text() + "s");
    }
}

CloneCommand::CloneCommand(UndoCommand *parent):
    UndoCommand(parent)
{

}

void CloneCommand::undo()
{
    for (quint64 id : cloneIdList)
    {
        auto clone = document()->drawingItem(id);
        if (clone == nullptr)
        {
            warnItemNotFound("Clone", id);
            continue;
        }
        document()->removeDrawingItem(id);
    }
}

void CloneCommand::redo()
{
    cloneIdList.clear();

    for (quint64 id : itemIdList)
    {
        auto item = document()->drawingItem(id);
        if (item == nullptr)
        {
            warnItemNotFound("Clone", id);
            continue;
        }
        auto clone = item->clone();
        quint64 cloneId = document()->addDrawingItem(clone);
        clone->position += translation;
        document()->updateDrawingItem(cloneId);
        cloneIdList.append(cloneId);
    }

    setText(QString("Clone %1 item").arg(itemIdList.count()));
    if (itemIdList.count() > 1)
    {
        setText(text() + "s");
    }
}
