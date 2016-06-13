#pragma once

#include <QGraphicsView>
#include "palette.h"

namespace SymbolEditor
{

    class Scene;
    class InteractiveTool;
    class Item;
    class Handle;
    class SnapManager;
    class RulerBarWidget;
    class Palette;
    class CartesianGrid;
    class Grid;
    class Settings;

    class View : public QGraphicsView
    {
        Q_OBJECT

    public:
        // TBD: Rename to cursor mark => origin, relative origin and cursor positions
        enum MouseCursor
        {
            NoMouseCursor,
            SmallMouseCursor,
            LargeMouseCursor
        };
        Q_ENUM(MouseCursor)

        enum OriginMark
        {
            NoOriginMark,
            SmallOriginMark,
            LargeOriginMark
        };
        Q_ENUM(OriginMark)

        explicit View(QWidget *parent = nullptr);
        ~View();

        Scene *scene() const;
        void setScene(Scene *scene);

        InteractiveTool *tool();
        void setTool(InteractiveTool *tool);

        Item *objectAt(const QPointF &pos) const;
        Handle *handleAt(const QPointF &pos) const;

        Handle *handleUnderMouse() const;
        Item *objectUnderMouse() const;
        QPointF mousePosition() const;
        QPointF cursorPosition() const;
        QPointF originPosition() const;

        void scaleView(qreal scaleFactor);
        void translateView(qreal dx, qreal dy);

        void setPalette(Palette palette);
        Palette palette() const;

        QSizeF pixelSize() const;

        const Grid *grid() const;
        SnapManager *snapManager();

        void zoomIn(QPointF pos, qreal factor);

        void setHardwareAccelerationEnabled(bool enabled);
        bool hardwareAccelerationEnabled() const;

        void setRulerEnabled(bool enabled);
        bool rulerEnabled() const;

        void setGridEnabled(bool enabled);
        bool gridEnabled() const;

        void setMinimalGridSize(int pixels);
        int minimalGridSize() const;

        void setGridCoarseMultiplier(int multiplier);
        int gridCoarseMultiplier() const;

        void setGridCoarseLineStyle(Qt::PenStyle style);
        Qt::PenStyle gridCoarseLineStyle() const;

        void setGridFineLineStyle(Qt::PenStyle style);
        Qt::PenStyle gridFineLineStyle() const;

        void setMouseCursor(MouseCursor cursor);
        MouseCursor mouseCursor() const;

        void setOriginMark(OriginMark mark);
        OriginMark originMark() const;

    public slots:
        void applySettings(const Settings &settings);

    protected:
        void drawBackground(QPainter *painter, const QRectF &rect);
        void drawForeground(QPainter *painter, const QRectF &rect);

        void wheelEvent(QWheelEvent *event);

        void mousePressEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        void mouseReleaseEvent(QMouseEvent *event);
        void mouseDoubleClickEvent(QMouseEvent *event);

        void keyPressEvent(QKeyEvent *event);
        void keyReleaseEvent(QKeyEvent *event);
        void resizeEvent(QResizeEvent *);

    private:
        bool m_hardwareAccelerationEnabled;
        bool m_rulerEnabled;
        bool m_gridEnabled;
        MouseCursor m_mouseCursor;
        OriginMark m_originMark;

        CartesianGrid *m_grid;

        QWidget *m_cornerWidget;
        RulerBarWidget *m_horizontalRuler;
        RulerBarWidget *m_verticalRuler;
        void updateRulerCursorPositions();
        void updateRulerCursorRanges();

        InteractiveTool *m_tool;
        QPointF m_mousePosition;
        bool m_mousePositionChanged;
        void updateMousePos();
        QPoint m_lastGlobalPos;

        SnapManager *m_snapManager;
        bool m_snapping;
        bool m_snapToGridEnabled;
        QMouseEvent createSnappedMouseEvent(QMouseEvent *event);

        QRectF visibleSceneRect() const;
        void updateForeground();
        void updateBackground();

        Palette m_palette;
        void applyPalette();

        bool m_panning;

        void startPanView(QMouseEvent *event);
        void updatePanView(QMouseEvent *event);
        void endPanView();
        void drawCursor(QPainter *painter);
        void drawOrigin(QPainter *painter);
        void drawSnapDecoration(QPainter *painter);
    };

}
