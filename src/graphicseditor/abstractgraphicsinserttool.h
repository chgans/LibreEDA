#ifndef ABSTRACTGRAPHICSINSERTTOOL_H
#define ABSTRACTGRAPHICSINSERTTOOL_H

#include "abstractgraphicsinteractivetool.h"

#include <QPointF>
#include <QString>

class AbstractGraphicsHandle;
class QWidget;
class QAction;

// TBD: ctrl+space: cycle mode
// Use template whith GraphicsXyzItem, or use GraphicsObject base class
// Should work as well for single point (either a "Point" object, or a fixed shape/size object)
// addPoint return a GraphicsHandle, movePoint has a default impl.
class AbstractGraphicsInsertTool : public AbstractGraphicsInteractiveTool
{
    Q_OBJECT
public:
    AbstractGraphicsInsertTool(QObject *parent = 0);
    ~AbstractGraphicsInsertTool();

    // Typ. mouse clicked, idx == 0
    virtual GraphicsObject *beginInsert(const QPointF &pos) = 0;
    // Typ. first mouse move after mouse clicked, idx > 0
    virtual void addPoint(int idx, const QPointF &pos) = 0;
    // Typ. mouse clicked, idx >= 0
    virtual void freezePoint(int idx, const QPointF &pos) = 0;
    // Typ. Esc pressed, idx > 0
    virtual bool removePoint(int idx, const QPointF &pos) = 0;
    // Typ. mouse move, idx > 0
    virtual void movePoint(int idx, const QPointF &pos) = 0;
    // Typ. mouse doubleclick
    virtual void endInsert(const QPointF &pos) = 0;
    // Typ. Esc pressed, idx > 0
    virtual void cancelInsert() = 0;

    // convenience functions
    QPointF mapToScene(const QPoint &pos);
    QPointF mapToItem(const QPoint &pos);
    QPointF mapFromScene(const QPointF &pos);
    QPointF mapFromItem(const QPointF &pos);

    void resetTool();

signals:
    void objectInserted(GraphicsObject *object);

public slots:
    void goBack();

private:
    int m_index;
    QPointF m_pressPos;
    QPointF m_movePos;
    bool m_addPointOnMouseMove;
    bool m_isActive;
    GraphicsObject *m_item;

    // AbstractGraphicsInteractiveTool interface
public:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);

    // GraphicsTool interface
public:
    void activate(const QAction *which, GraphicsView *view);
    void desactivate(const QAction *which, GraphicsView *view);

};

#endif // ABSTRACTGRAPHICSINSERTTOOL_H
