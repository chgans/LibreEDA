#ifndef PLACEITEMCOMMAND_H
#define PLACEITEMCOMMAND_H

#include <QUndoCommand>
#include <QPen>
#include <QPointF>
#include <QFont>

#include "xdl/symbol.h"

namespace SymbolEditor
{

    class SchEditorDocument;

    class UndoCommand: public QUndoCommand
    {
    public:
        UndoCommand(UndoCommand *parent = nullptr);

        void setDocument(SchEditorDocument *document);
        SchEditorDocument *document() const;

    protected:
        void warnItemNotFound(const QString command, quint64 id);

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

    class ChangeRectangleGeometryCommand: public PlacementCommand
    {
    public:
        ChangeRectangleGeometryCommand(UndoCommand *parent = nullptr);

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

    class ChangeCircleGeometryCommand: public PlacementCommand
    {
    public:
        ChangeCircleGeometryCommand(UndoCommand *parent = nullptr);

        void undo();
        void redo();

        QPointF center; // = position ?
        qreal radius;
    };

    class PlaceCircularArcCommand: public PlacementCommand
    {
    public:
        PlaceCircularArcCommand(UndoCommand *parent = nullptr);

        void undo();
        void redo();

        QPointF center; // = position ?
        qreal radius;
        qreal startAngle;
        qreal spanAngle;
    };

    class ChangeCircularArcGeometryCommand: public PlacementCommand
    {
    public:
        ChangeCircularArcGeometryCommand(UndoCommand *parent = nullptr);

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

    class ChangeEllipseGeometryCommand: public PlacementCommand
    {
    public:
        ChangeEllipseGeometryCommand(UndoCommand *parent = nullptr);

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

    class ChangeEllipticalArcGeometryCommand: public PlacementCommand
    {
    public:
        ChangeEllipticalArcGeometryCommand(UndoCommand *parent = nullptr);

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

    class ChangePolylineGeometryCommand: public PlacementCommand
    {
    public:
        ChangePolylineGeometryCommand(UndoCommand *parent = nullptr);

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

    class ChangePolygonGeometryCommand: public PlacementCommand
    {
    public:
        ChangePolygonGeometryCommand(UndoCommand *parent = nullptr);

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

    class ChangeLabelContentCommand: public UndoCommand
    {
    public:
        ChangeLabelContentCommand(UndoCommand *parent = nullptr);

        void undo();
        void redo();

        QString text;
        QFont font;
    };

    // TODO: PlacePinCommand

    // TBD: align and distribute
    class TranslateCommand: public UndoCommand
    {
    public:
        TranslateCommand(UndoCommand *parent = nullptr);

        void undo();
        void redo();

        QList<quint64> itemIdList;
        QPointF amount;
    };

    class RotateCommand: public UndoCommand
    {
    public:
        RotateCommand(UndoCommand *parent = nullptr);

        void undo();
        void redo();

        QList<quint64> itemIdList;
        qreal amount;
    };

    class MirrorCommand: public UndoCommand
    {
    public:
        MirrorCommand(UndoCommand *parent = nullptr);

        void undo();
        void redo();

        QList<quint64> itemIdList;
        Qt::Orientation orientation;
        bool mirrored;
    };

    class SetLockStateCommand: public UndoCommand
    {
    public:
        SetLockStateCommand(UndoCommand *parent = nullptr);

        void undo();
        void redo();

        QList<quint64> itemIdList;
        bool lockState;
    };

    class SetVisibilityCommand: public UndoCommand
    {
    public:
        SetVisibilityCommand(UndoCommand *parent = nullptr);

        void undo();
        void redo();

        QList<quint64> itemIdList;
        bool visibility;
    };

    class ChangeOpacityByCommand: public UndoCommand
    {
    public:
        ChangeOpacityByCommand(UndoCommand *parent = nullptr);

        void undo();
        void redo();

        qreal amount;
    };

    class ChangeZValueByCommand: public UndoCommand
    {
    public:
        ChangeZValueByCommand(UndoCommand *parent = nullptr);

        void undo();
        void redo();

        QList<quint64> itemIdList;
        qreal amount;
    };

    // group/ungroup

    class CloneCommand: public UndoCommand
    {
    public:
        CloneCommand(UndoCommand *parent = nullptr);

        void undo();
        void redo();

        QList<quint64> itemIdList;
        QPointF translation;

    private:
        QList<quint64> cloneIdList;

    };

}

#endif // PLACEITEMCOMMAND_H
