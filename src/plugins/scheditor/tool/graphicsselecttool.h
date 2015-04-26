#ifndef GRAPHICSSELECTTOOL_H
#define GRAPHICSSELECTTOOL_H

#include "abstractgraphicsinteractivetool.h"

#include <QPoint>

class SchItem;
class AbstractGraphicsHandle;

class QRubberBand;

class GraphicsSelectTool : public AbstractGraphicsInteractiveTool
{
    Q_OBJECT

public:
    GraphicsSelectTool(QObject *parent = 0);
    ~GraphicsSelectTool();

private:
    enum ToolState {
        HintState,
        OperationState
    };
    enum Operation {
        DragSelect,
        //ClickSelect,
        MoveItem,
        MoveHandle,
        CloneItem
    };
    ToolState m_state;
    Operation m_operation;

    QPoint m_mousePressPosition;
    SchItem *m_item;
    QList<SchItem *> m_items;
    QList<SchItem *> m_phantomItems;
    AbstractGraphicsHandle *m_handle;
    QRubberBand *m_rubberBand;

    void updateCursor(QMouseEvent *event);
    void setOperation(Operation operation);

    // GraphicsTool interface
public:
    void setView(SchView *view);
    virtual void activate(const QAction *which, SchView *view);
    virtual void desactivate(const QAction *which, SchView *view);

    // GraphicsTool interface
public slots:
    virtual void cancel();

    // GraphicsTool interface
public:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void keyReleaseEvent(QKeyEvent *event);
};

#endif // GRAPHICSSELECTTOOL_H
