#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>

class GraphicsScene;
class AbstractGraphicsInteractiveTool;
class GraphicsObject;
class GraphicsHandle;

class GraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    GraphicsView(QWidget *parent = 0);
    ~GraphicsView();

    GraphicsScene *scene();
    void setScene(GraphicsScene *scene);

    AbstractGraphicsInteractiveTool *tool();
    void setTool(AbstractGraphicsInteractiveTool *tool);

    GraphicsObject *objectAt(const QPoint &pos) const;
    GraphicsHandle *handleAt(const QPoint &pos) const;

    GraphicsHandle *handleUnderMouse() const;
    GraphicsObject *objectUnderMouse() const;
    QPoint mousePosition() const;

    void scaleView(qreal scaleFactor);

public slots:
    void enableSnapToGrid(bool enabled);
    bool isSnapToGridEnabled() const;

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

private:
    AbstractGraphicsInteractiveTool *m_tool;
    GraphicsObject *m_objectUnderMouse;
    const GraphicsHandle *m_handleUnderMouse;
    QPoint m_mousePosition;
    bool m_mousePositionChanged;
    void updateMousePos();
    QMouseEvent snapMouseEvent(QMouseEvent *event);
    QSizeF pixelSize() const;
    bool m_snapToGridEnabled;
};

#endif // GRAPHICSVIEW_H
