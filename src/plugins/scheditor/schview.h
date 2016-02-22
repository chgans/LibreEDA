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

class SchView : public QGraphicsView
{
    Q_OBJECT

public:
    SchView(QWidget *parent = 0);
    ~SchView();

    SchScene *scene();
    void setScene(SchScene *scene);

    AbstractGraphicsInteractiveTool *tool();
    void setTool(AbstractGraphicsInteractiveTool *tool);

    SchItem *objectAt(const QPoint &pos) const;
    AbstractGraphicsHandle *handleAt(const QPoint &pos) const;

    AbstractGraphicsHandle *handleUnderMouse() const;
    SchItem *objectUnderMouse() const;
    QPoint mousePosition() const;

    void scaleView(qreal scaleFactor);
    void translateView(qreal dx, qreal dy);

    void setPaletteMode(Palette::Mode mode);
    Palette::Mode paletteMode() const;

    const Palette *palette() const;
    QSizeF pixelSize() const;

    SnapManager *snapManager();


    void zoomIn(QPointF pos, qreal factor);

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
    QWidget *m_cornerWidget;
    GraphicsViewRuler *m_horizontalRuler;
    GraphicsViewRuler *m_verticalRuler;
    void updateRulerCursorPositions();
    void updateRulerCursorRanges();

    AbstractGraphicsInteractiveTool *m_tool;
    SchItem *m_objectUnderMouse;
    const AbstractGraphicsHandle *m_handleUnderMouse;
    QPoint m_mousePosition;
    bool m_mousePositionChanged;
    void updateMousePos();
    QPoint m_lastGlobalPos;

    SnapManager *m_snapManager;
    bool m_snapping;
    bool m_snapToGridEnabled;
    QMouseEvent createSnappedMouseEvent(QMouseEvent *event);

    Palette *m_palette;
    void applyPalette();

    bool m_panning;

    bool m_openGLEnabled;

    void startPanView(QMouseEvent *event);
    void updatePanView(QMouseEvent *event);
    void endPanView();
};

#endif // GRAPHICSVIEW_H
