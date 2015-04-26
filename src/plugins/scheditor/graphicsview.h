#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>

class SchScene;
class AbstractGraphicsInteractiveTool;
class SchItem;
class AbstractGraphicsHandle;

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
    SchItem *m_objectUnderMouse;
    const AbstractGraphicsHandle *m_handleUnderMouse;
    QPoint m_mousePosition;
    bool m_mousePositionChanged;
    void updateMousePos();
    QMouseEvent snapMouseEvent(QMouseEvent *event);
    QSizeF pixelSize() const;
    bool m_snapToGridEnabled;
};

#endif // GRAPHICSVIEW_H
