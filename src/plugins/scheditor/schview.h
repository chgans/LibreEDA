#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>
#include "palette.h"

class SchScene;
class AbstractGraphicsInteractiveTool;
class SchItem;
class AbstractGraphicsHandle;
class SnapManager;
class GraphicsViewRuler;
class Palette;
class GraphicsCartesianGrid;
class GraphicsGrid;

class SchView : public QGraphicsView
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

    SchView(QWidget *parent = 0);
    ~SchView();

    SchScene *scene() const;
    void setScene(SchScene *scene);

    AbstractGraphicsInteractiveTool *tool();
    void setTool(AbstractGraphicsInteractiveTool *tool);

    SchItem *objectAt(const QPointF &pos) const;
    AbstractGraphicsHandle *handleAt(const QPointF &pos) const;

    AbstractGraphicsHandle *handleUnderMouse() const;
    SchItem *objectUnderMouse() const;
    QPointF mousePosition() const;
    QPointF cursorPosition() const;
    QPointF originPosition() const;

    void scaleView(qreal scaleFactor);
    void translateView(qreal dx, qreal dy);

    void setPaletteMode(Palette::Mode mode);
    Palette::Mode paletteMode() const;

    const Palette *palette() const;
    QSizeF pixelSize() const;

    const GraphicsGrid *grid() const;
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

private:

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

    GraphicsCartesianGrid *m_grid;

    QWidget *m_cornerWidget;
    GraphicsViewRuler *m_horizontalRuler;
    GraphicsViewRuler *m_verticalRuler;
    void updateRulerCursorPositions();
    void updateRulerCursorRanges();

    AbstractGraphicsInteractiveTool *m_tool;
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

    Palette *m_palette;
    void applyPalette();

    bool m_panning;

    void startPanView(QMouseEvent *event);
    void updatePanView(QMouseEvent *event);
    void endPanView();
    void drawCursor(QPainter *painter);
    void drawOrigin(QPainter *painter);
    void drawSnapDecoration(QPainter *painter);
};

#endif // GRAPHICSVIEW_H
