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
    GraphicsViewRuler *m_hViewRuler;
    GraphicsViewRuler *m_vViewRuler;
    AbstractGraphicsInteractiveTool *m_tool;
    SchItem *m_objectUnderMouse;
    const AbstractGraphicsHandle *m_handleUnderMouse;
    QPoint m_mousePosition;
    bool m_mousePositionChanged;
    void updateMousePos();
    QMouseEvent snapMouseEvent(QMouseEvent *event);
    bool m_snapToGridEnabled;
    Palette *m_palette;
    void applyPalette();
    SnapManager *m_snapManager;
    bool m_snapping;

    bool m_panning = false;
    QPoint m_lastGlobalPos;

};

#endif // GRAPHICSVIEW_H
