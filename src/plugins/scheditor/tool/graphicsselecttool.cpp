#include "tool/graphicsselecttool.h"

#include "schscene.h"
#include "schview.h"
#include "item/schitem.h"
#include "handle/abstractgraphicshandle.h"

#include <QGraphicsItem>
#include <QGraphicsEffect>

#include <QMouseEvent>
#include <QKeyEvent>
#include <QRubberBand>
#include <QAction>
#include <QDialog>

#include <QStateMachine>
#include <QState>
#include <QFinalState>
#include <QSignalTransition>

#include <QDebug>

// Select tool:
//  Click based:
//  - clear selection: left press on background, release
//  - add/remove item to selection: left press on item w or w/o Shift modifier, release
//  - double left click => send signal
//  - right click => send signal (done by QWidget)
//  Drag based:
//  - drag select: left press on background, drag mouse, release
//  - single move: left press on item, drag mouse, release
//  - group move: left press on any selected items, drag mouse, release
//  - clone: left press w/ Ctrl on item, drag mouse, release
//  Cursors:
//   Qt::DragMoveCursor
//   Qt::DragCopyCursor
//   Qt::SizeAllCursor

GraphicsSelectTool::GraphicsSelectTool(QObject *parent):
    AbstractGraphicsInteractiveTool(parent),
    m_state(HintState),
    m_operation(DragSelect),
    m_mousePressPosition(QPoint(0, 0)),
    m_item(nullptr),
    m_handle(nullptr),
    m_rubberBand(new QRubberBand(QRubberBand::Rectangle))
{
    QAction *action = new QAction(QIcon::fromTheme("edit-select"),
                           "select", nullptr);
    setAction(action);
    setToolGroup("interactive-tools");
    setOperationWidget(nullptr);
    setOptionWidget(nullptr);
}

GraphicsSelectTool::~GraphicsSelectTool()
{

}

void GraphicsSelectTool::updateCursor(QMouseEvent *event)
{
    AbstractGraphicsHandle *handle = view()->handleUnderMouse();
    SchItem *object = view()->objectUnderMouse();
    if (handle != nullptr) {
        m_handle = handle;
        setOperation(MoveHandle);
    }
    else if (object != nullptr) {
        if (event->modifiers().testFlag(Qt::ControlModifier))
            setOperation(CloneItem);
        else
            setOperation(MoveItem);
    }
    else
        setOperation(DragSelect);
}

void GraphicsSelectTool::setOperation(GraphicsSelectTool::Operation operation)
{
    m_operation = operation;
    switch (operation) {
    case DragSelect:
        view()->setCursor(Qt::ArrowCursor);
        break;
    case MoveItem:
        view()->setCursor(Qt::SizeAllCursor);
        break;
    case MoveHandle:
        Q_ASSERT(m_handle != nullptr);
        // TBD: Cursor can change when the handle is dragged around
        // it would be nice to update the view cursor accordingly
        view()->setCursor(m_handle->handleCursor());
        break;
    case CloneItem:
        view()->setCursor(Qt::DragCopyCursor);
        break;
    default:
        break;
    }
}

void GraphicsSelectTool::activate(const QAction *which, SchView *view)
{
    Q_UNUSED(which);
    Q_UNUSED(view);
}

void GraphicsSelectTool::desactivate(const QAction *which, SchView *view)
{
    Q_UNUSED(which);
    Q_UNUSED(view);
}

void GraphicsSelectTool::cancel()
{
}

void GraphicsSelectTool::setView(SchView *other)
{
    if (view()) {
        //destroyStateMachine();
        m_rubberBand->setParent(nullptr);
    }
    SchTool::setView(other);
    if (other) {
        //buildStateMachine();
        m_rubberBand->setParent(view());
    }
}

void GraphicsSelectTool::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
        return;

    if (m_state == HintState) {
        AbstractGraphicsHandle *handle = view()->handleUnderMouse();
        SchItem *object = view()->objectUnderMouse();
        m_mousePressPosition = event->pos();
        switch (m_operation) {
        case DragSelect:
            if (!event->modifiers().testFlag(Qt::ShiftModifier))
                scene()->clearSelection();
            m_rubberBand->setGeometry(QRect(m_mousePressPosition, m_mousePressPosition));
            m_rubberBand->show();
            break;
        case MoveItem:
        case CloneItem:
            Q_ASSERT(object != nullptr);
            if (event->modifiers().testFlag(Qt::ShiftModifier)) {
                object->setSelected(!object->isSelected());
            }
            else if (!object->isSelected()) {
                scene()->clearSelection();
                object->setSelected(true);
            }
            m_item = object;
            m_items = scene()->selectedObjects();
            break;
        case MoveHandle:
            Q_ASSERT(handle != nullptr);
            m_handle = handle;
            break;
        default:
            break;
        }
        m_state = OperationState;
    }
    event->accept();
}

void GraphicsSelectTool::mouseMoveEvent(QMouseEvent *event)
{
    if (m_state == HintState) {
        updateCursor(event);
    }
    else {
        switch (m_operation) {
        case DragSelect: {
            QRect viewRect = QRect(m_mousePressPosition, event->pos()).normalized();
            m_rubberBand->setGeometry(viewRect);
            QPainterPath path;
            path.addPolygon(view()->mapToScene(viewRect));
            scene()->setSelectionArea(path);
            break;
        }
        case MoveItem:
        case CloneItem: {
            if (m_phantomItems.count() == 0) {
                m_phantomItems = createPhantomItems(m_items);
            }
            Q_ASSERT(m_items.count() == m_phantomItems.count());
            QRectF sceneShift = QRectF(view()->mapToScene(m_mousePressPosition),
                                       view()->mapToScene(event->pos()));
            sceneShift.moveTopLeft(QPointF(0, 0));
            //qDebug() << event->pos() << m_mousePressPosition << viewVector << sceneVector;
            for (int i = 0; i < m_items.count(); i++) {
                QPointF itemPos = m_items[i]->pos() + sceneShift.bottomRight();
                m_phantomItems[i]->setPos(itemPos);
            }
            break;
        }
        case MoveHandle: {
            // TODO: use phantomItem as well
            QPointF scenePos = view()->mapToScene(event->pos());
            QPointF handlePos = m_handle->mapToParent(m_handle->mapFromScene(scenePos));
            m_handle->setPos(handlePos);
            break;
        }
        default:
            qDebug() << "Unknown state";
        }
    }
    event->accept();
}

void GraphicsSelectTool::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
        return;

    switch (m_operation) {
    case DragSelect:
        m_rubberBand->hide();
        break;
    case MoveItem:
        if (m_phantomItems.count() > 0) {
            for (int i = 0; i < m_items.count(); i++) {
                m_items[i]->setGraphicsEffect(nullptr);
                m_items[i]->setPos(m_phantomItems[i]->pos());
                scene()->removeItem(m_phantomItems[i]);
                delete m_phantomItems[i];
            }
            m_phantomItems.clear();
        }
        m_items.clear();
        m_item = nullptr;
        break;
    case CloneItem:
        for (int i = 0; i < m_items.count(); i++) {
            m_items[i]->setGraphicsEffect(nullptr);
            m_phantomItems[i]->setGraphicsEffect(nullptr);
        }
        m_phantomItems.clear();
        m_items.clear();
        m_item = nullptr;
        break;
    case MoveHandle:
        m_handle = nullptr;
    }

    m_state = HintState;

    updateCursor(event);
    event->accept();
}

void GraphicsSelectTool::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Control) {
        if (m_operation == MoveItem && m_state == HintState)
            setOperation(CloneItem);
    }
    /*
    else if (event->key() == Qt::Key_Delete) {
        QPoint pos = view()->mapFromGlobal(QCursor::pos());
        GraphicsObject *object = view()->objectAt(pos);
        const GraphicsHandle *handle = view()->handleAt(pos);
        if (handle != nullptr) {
            object->removeHandle(object->handles().indexOf(handle));
        }

    }
    */
    event->accept();
}

void GraphicsSelectTool::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Control) {
        if (m_operation == CloneItem && m_state == HintState)
            setOperation(MoveItem);
    }
    event->accept();
}
