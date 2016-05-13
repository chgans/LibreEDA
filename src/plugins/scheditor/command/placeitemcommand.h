#ifndef PLACEITEMCOMMAND_H
#define PLACEITEMCOMMAND_H

#include <QUndoCommand>
#include <QPen>
#include <QPointF>
#include <QFont>

#include "xdl/symbol.h"

class SchEditorDocument;

class UndoCommand: public QUndoCommand
{
public:
    UndoCommand(UndoCommand *parent = nullptr);

    void setDocument(SchEditorDocument *document);
    SchEditorDocument *document() const;

private:
    SchEditorDocument *m_document;
};

class PlacementCommand: public UndoCommand
{
public:
    PlacementCommand(UndoCommand *parent = nullptr);

protected:
    void removeItem();
    void placeItem(xdl::symbol::Item *item);

public:
    quint64 itemId;

    // 1 - Visual
    QPen pen;
    QBrush brush;
    qreal opacity;
    // 2 - Geometry
    QPointF position;
    qreal rotation;
    bool xMirrored;
    bool yMirrored;
    // 3 - State
    bool locked;
    bool visible;
    qreal zValue;
};

class PlaceRectangleCommand: public PlacementCommand
{
public:
    PlaceRectangleCommand(UndoCommand *parent = nullptr);

    void undo();
    void redo();

    QPointF topLeft;
    QPointF bottomRight;
};

class PlaceCircleCommand: public PlacementCommand
{
public:
    PlaceCircleCommand(UndoCommand *parent = nullptr);

    void undo();
    void redo();

    QPointF center; // = position ?
    qreal radius;
};

class PlaceCircularCommand: public PlacementCommand
{
public:
    PlaceCircularCommand(UndoCommand *parent = nullptr);

    void undo();
    void redo();

    QPointF center; // = position ?
    qreal radius;
    qreal startAngle;
    qreal spanAngle;
};

class PlaceEllipseCommand: public PlacementCommand
{
public:
    PlaceEllipseCommand(UndoCommand *parent = nullptr);

    void undo();
    void redo();

    QPointF center; // = position ?
    qreal xRadius;
    qreal yRadius;
};

class PlaceEllipticalArcCommand: public PlacementCommand
{
public:
    PlaceEllipticalArcCommand(UndoCommand *parent = nullptr);

    void undo();
    void redo();

    QPointF center; // = position ?
    qreal xRadius;
    qreal yRadius;
    qreal startAngle;
    qreal spanAngle;
};

class PlacePolylineCommand: public PlacementCommand
{
public:
    PlacePolylineCommand(UndoCommand *parent = nullptr);

    void undo();
    void redo();

    QList<QPointF> vertices; // position is always first point?
};

class PlacePolygonCommand: public PlacementCommand
{
public:
    PlacePolygonCommand(UndoCommand *parent = nullptr);

    void undo();
    void redo();

    QList<QPointF> vertices; // position is always first point?
};

class PlaceLabelCommand: public PlacementCommand
{
public:
    PlaceLabelCommand(UndoCommand *parent = nullptr);

    void undo();
    void redo();

    QString text;
    QFont font;
};

#endif // PLACEITEMCOMMAND_H
