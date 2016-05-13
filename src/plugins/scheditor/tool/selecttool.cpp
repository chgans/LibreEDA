#include "tool/selecttool.h"

#include "schscene.h"
#include "schview.h"
#include "item/schitem.h"
#include "handle/abstractgraphicshandle.h"
#include "command/placeitemcommand.h"

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

SelectTool::SelectTool(QObject *parent):
    InteractiveTool(parent),
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

SelectTool::~SelectTool()
{

}

QPointF SelectTool::mouseDeltaPosition() const
{
    return m_lastMousePosition - m_mousePressPosition;
}

void SelectTool::updateCursor(QMouseEvent *event)
{
    AbstractGraphicsHandle *handle = view()->handleUnderMouse();
    SchItem *object = view()->objectUnderMouse();
    if (handle != nullptr)
    {
        m_handle = handle;
        setOperation(MoveHandle);
    }
    else if (object != nullptr && object->isEnabled())
    {
        if (event->modifiers().testFlag(Qt::ControlModifier))
        {
            setOperation(CloneItem);
        }
        else
        {
            setOperation(MoveItem);
        }
    }
    else
    {
        setOperation(DragSelect);
    }
}

void SelectTool::setOperation(SelectTool::Operation operation)
{
    m_operation = operation;
    switch (operation)
    {
        case DragSelect:
        {
            view()->setCursor(Qt::ArrowCursor);
            break;
        }
        case MoveItem:
        {
            view()->setCursor(Qt::SizeAllCursor);
            break;
        }
        case MoveHandle:
        {
            Q_ASSERT(m_handle != nullptr);
            // TBD: Cursor can change when the handle is dragged around
            // it would be nice to update the view cursor accordingly
            view()->setCursor(m_handle->handleCursor());
            break;
        }
        case CloneItem:
        {
            view()->setCursor(Qt::DragCopyCursor);
            break;
        }
    }
}

void SelectTool::cancel()
{
}

void SelectTool::setView(SchView *other)
{
    if (view())
    {
        //destroyStateMachine();
        m_rubberBand->setParent(nullptr);
    }
    SchTool::setView(other);
    if (other)
    {
        //buildStateMachine();
        m_rubberBand->setParent(view()->viewport());
    }
}

void SelectTool::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
    {
        return;
    }

    if (m_state == HintState)
    {
        AbstractGraphicsHandle *handle = view()->handleUnderMouse();
        SchItem *object = view()->objectUnderMouse();
        m_mousePressPosition = event->pos();
        switch (m_operation)
        {
            case DragSelect:
            {
                if (!event->modifiers().testFlag(Qt::ShiftModifier))
                {
                    scene()->clearSelection();
                }
                m_rubberBand->setGeometry(view()->mapFromScene(QRect(m_mousePressPosition,
                                                                     m_mousePressPosition)).boundingRect());
                m_rubberBand->show();
                break;
            }
            case MoveItem:
            case CloneItem:
            {
                Q_ASSERT(object != nullptr);
                if (event->modifiers().testFlag(Qt::ShiftModifier))
                {
                    object->setSelected(!object->isSelected());
                }
                else if (!object->isSelected())
                {
                    scene()->clearSelection();
                    object->setSelected(true);
                }
                m_item = object;
                m_items = scene()->selectedObjects();
                m_lastMousePosition = event->pos();
                break;
            }
            case MoveHandle:
            {
                Q_ASSERT(handle != nullptr);
                m_handle = handle;
                break;
            }
        }
        m_state = OperationState;
    }
    event->accept();
}

// TODO: All these operation should be cancelable using 'Esc' key
void SelectTool::mouseMoveEvent(QMouseEvent *event)
{
    if (m_state == HintState)
    {
        updateCursor(event);
    }
    else
    {
        switch (m_operation)
        {
            case DragSelect:
            {
                QRect viewRect = view()->mapFromScene(QRect(m_mousePressPosition, event->pos())).boundingRect();
                m_rubberBand->setGeometry(viewRect);
                break;
            }
            case MoveItem:
            {
                QPointF mouseScenePosition = event->pos();
                QLineF vector(m_lastMousePosition, mouseScenePosition);
                for (int i = 0; i < m_items.count(); i++)
                {
                    m_items[i]->moveBy(vector.dx(), vector.dy());
                }
                m_lastMousePosition = mouseScenePosition;
                break;
            }
            case CloneItem:
            {
                if (m_phantomItems.count() == 0)
                {
                    m_phantomItems = createPhantomItems(m_items);
                }
                Q_ASSERT(m_items.count() == m_phantomItems.count());
                QRectF sceneShift = QRectF(m_mousePressPosition,
                                           event->pos());
                sceneShift.moveTopLeft(QPointF(0, 0));
                for (int i = 0; i < m_items.count(); i++)
                {
                    QPointF itemPos = m_items[i]->pos() + sceneShift.bottomRight();
                    m_phantomItems[i]->setPos(itemPos);
                }
                break;
            }
            case MoveHandle:
            {
                // TODO: use phantomItem as well
                QPointF scenePos = event->pos();
                QPointF handlePos = m_handle->mapToParent(m_handle->mapFromScene(scenePos));
                m_handle->setPos(handlePos);
                break;
            }
        }
    }
    event->accept();
}

void SelectTool::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
    {
        return;
    }

    switch (m_operation)
    {
        case DragSelect:
        {
            QPainterPath path;
            path.addPolygon(view()->mapToScene(m_rubberBand->geometry()));
            scene()->setSelectionArea(path);
            m_rubberBand->hide();
            break;
        }
        case MoveItem:
        {
            QPointF delta = mouseDeltaPosition();
            if (!delta.isNull())
            {
                auto command = new MoveCommand;
                for (auto sceneItem : m_items)
                {
                    command->itemIds << sceneItem->data(0).value<quint64>();
                    command->delta = delta;
                }
                emit taskCompleted(command);
            }
            m_items.clear();
            m_item = nullptr;
            break;
        }
        case CloneItem:
        {
            for (int i = 0; i < m_items.count(); i++)
            {
                m_items[i]->setGraphicsEffect(nullptr);
                m_phantomItems[i]->setGraphicsEffect(nullptr);
            }
            m_phantomItems.clear();
            m_items.clear();
            m_item = nullptr;
            break;
        }
        case MoveHandle:
            m_handle = nullptr;
    }

    m_state = HintState;

    updateCursor(event);
    event->accept();
}

void SelectTool::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Control)
    {
        if (m_operation == MoveItem && m_state == HintState)
        {
            setOperation(CloneItem);
        }
    }
    else if (event->key() == Qt::Key_Delete)
    {
        for (QGraphicsItem *item : scene()->selectedItems())
        {
            scene()->removeItem(item);
            delete item;
        }
    }
    event->accept();
}

void SelectTool::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Control)
    {
        if (m_operation == CloneItem && m_state == HintState)
        {
            setOperation(MoveItem);
        }
    }
    event->accept();
}
