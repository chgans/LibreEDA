#ifndef PLACEITEMCOMMAND_H
#define PLACEITEMCOMMAND_H

#include <QUndoCommand>
#include <QPen>
#include <QPointF>
#include <QFont>

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

class PlaceItemCommand: public UndoCommand
{
public:
    PlaceItemCommand(UndoCommand *parent = nullptr);

    void undo();
    void redo();

    // 1 - Visual
    qreal penWidth;
    Qt::PenStyle penStyle;
    QColor penColor;
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

class ChangeItemVisual: public UndoCommand
{
public:
    ChangeItemVisual(UndoCommand *parent = nullptr);

    void undo();
    void redo();

    qreal penWidth;
    Qt::PenStyle penStyle;
    QColor penColor;
    qreal opacity;
};

class ChangeItemGeometry: public UndoCommand
{
public:
    ChangeItemGeometry(UndoCommand *parent = nullptr);

    void undo();
    void redo();

    QPointF position;
    qreal rotation;
    bool xMirrored;
    bool yMirrored;
};

class ChangeItemState: public UndoCommand
{
public:
    ChangeItemState(UndoCommand *parent = nullptr);

    bool locked;
    bool visible;
    qreal zValue;
};

class PlaceRectangleCommand: public PlaceItemCommand
{
public:
    PlaceRectangleCommand(UndoCommand *parent = nullptr);

    void undo();
    void redo();

    QPointF topLeft;
    QPointF bottomRight;
};

class PlaceCircleCommand: public PlaceItemCommand
{
public:
    PlaceCircleCommand(UndoCommand *parent = nullptr);

    void undo();
    void redo();

    QPointF center; // = position ?
    qreal radius;
};

class PlaceCircularCommand: public PlaceItemCommand
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

class PlaceEllipseCommand: public PlaceItemCommand
{
public:
    PlaceEllipseCommand(UndoCommand *parent = nullptr);

    void undo();
    void redo();

    QPointF center; // = position ?
    qreal xRadius;
    qreal yRadius;
};

class PlaceEllipticalArcCommand: public PlaceItemCommand
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

class PlacePolylineCommand: public PlaceItemCommand
{
public:
    PlacePolylineCommand(UndoCommand *parent = nullptr);

    void undo();
    void redo();

    QList<QPointF> vertices; // position is always first point?
};

class PlacePolygonCommand: public PlaceItemCommand
{
public:
    PlacePolygonCommand(UndoCommand *parent = nullptr);

    void undo();
    void redo();

    QList<QPointF> vertices; // position is always first point?
};

class PlaceLabelCommand: public PlaceItemCommand
{
public:
    PlaceLabelCommand(UndoCommand *parent = nullptr);

    void undo();
    void redo();

    QString text;
    QFont font;
};

#endif // PLACEITEMCOMMAND_H
