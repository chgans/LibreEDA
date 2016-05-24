#include "tool/selecttool.h"
#include "tool/moveitemtool.h"
#include "tool/cloneitemtool.h"
#include "tool/dragselecttool.h"
#include "scene.h"
#include "view.h"
#include "item/item.h"
#include "handle/handle.h"
#include "propertyeditor/itempropertyeditor.h"
#include "command/placeitemcommand.h"

#include <QMouseEvent>
#include <QKeyEvent>
#include <QAction>

SelectTool::SelectTool(QObject *parent):
    InteractiveTool(parent),
    m_state(HintState),
    m_operation(DragSelect),
    m_handle(nullptr)
{
    QAction *action = new QAction(QIcon::fromTheme("edit-select"),
                                  "select", nullptr);
    setAction(action);
    setToolGroup("interactive-tools");

    m_itemPropertyEditor = new ItemPropertyEditor;
    m_defaultTaskWidgets << m_itemPropertyEditor;

    m_moveItemTool = new MoveItemTool(this);
    connect(m_moveItemTool, &Tool::taskCompleted,
            this, &Tool::taskCompleted);

    m_cloneItemTool = new CloneItemTool(this);
    connect(m_cloneItemTool, &Tool::taskCompleted,
            this, &Tool::taskCompleted);

    m_dragSelectTool = new DragSelectTool(this);

    m_currentTool = nullptr;
    updateTaskWidgets();
}

SelectTool::~SelectTool()
{

}

void SelectTool::updateOperation(Qt::KeyboardModifiers modifiers)
{
    // Could use auto handle = view()->ItemUnderMouse<AbstractGraphicsHandle>();
    Handle *handle = view()->handleUnderMouse();
    if (handle != nullptr)
    {
        m_handle = handle;
        setOperation(MoveHandle);
        return;
    }

    // Could use auto item = view()->ItemUnderMouse<SchItem>();
    Item *item = view()->objectUnderMouse();
    if (item != nullptr && item->isEnabled())
    {
        if (modifiers.testFlag(Qt::ControlModifier))
        {
            setOperation(CloneItem);
        }
        else
        {
            setOperation(MoveItem);
        }
        return;
    }

    setOperation(DragSelect);
}

void SelectTool::updateCursor()
{
    switch (m_operation)
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

void SelectTool::setOperation(SelectTool::Operation operation)
{
    m_operation = operation;
    updateCursor();
}

void SelectTool::setCurrentTool(InteractiveTool *tool)
{
    if (m_currentTool != nullptr)
    {
        m_currentTool->setView(nullptr);
    }

    m_currentTool = tool;

    if (m_currentTool != nullptr)
    {
        m_currentTool->setView(view());
    }
    updateTaskWidgets();
}

void SelectTool::updateTaskWidgets()
{
    if (m_currentTool != nullptr)
    {
        setTaskWidgets(m_currentTool->taskWidgets());
    }
    else
    {
        setTaskWidgets(m_defaultTaskWidgets);
    }
}

void SelectTool::cancel()
{
}

void SelectTool::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
    {
        return;
    }

    Q_ASSERT(m_state == HintState);


    switch (m_operation)
    {
        case DragSelect:
        {
            if (!event->modifiers().testFlag(Qt::ShiftModifier))
            {
                scene()->clearSelection();
            }
            setCurrentTool(m_dragSelectTool);
            break;
        }
        case MoveItem:
        {
            Item *object = view()->objectUnderMouse();
            if (object == nullptr)
            {
                scene()->clearSelection();
            }
            else if (event->modifiers().testFlag(Qt::ShiftModifier))
            {
                object->setSelected(!object->isSelected());
            }
            else if (!object->isSelected())
            {
                scene()->clearSelection();
                object->setSelected(true);
                setCurrentTool(m_moveItemTool);
            }
            else
            {
                setCurrentTool(m_moveItemTool);
            }
            break;
        }
        case CloneItem:
        {
            setCurrentTool(m_cloneItemTool);
            break;
        }
        case MoveHandle:
        {
#if 0
            Q_ASSERT(handle != nullptr);
            m_handle = handle;
#endif
            break;
        }
    }

    m_state = OperationState;

    if (m_currentTool == nullptr)
    {
        return;
    }

    m_currentTool->mousePressEvent(event);

    event->accept();
}

// TODO: All these operation should be cancelable using 'Esc' key
void SelectTool::mouseMoveEvent(QMouseEvent *event)
{
    if (m_state == HintState)
    {
        updateOperation(event->modifiers());
        return;
    }

    if (m_operation == MoveHandle)
    {
    }
    else
    {
        m_currentTool->mouseMoveEvent(event);
    }
#if 0
    case MoveHandle:
    {
        // TBD: Cursor can change when the handle is dragged around
        // it would be nice to update the view cursor accordingly
        // TODO: use phantomItem as well
        QPointF scenePos = event->pos();
        QPointF handlePos = m_handle->mapToParent(m_handle->mapFromScene(scenePos));
        m_handle->setPos(handlePos);
        break;
    }
#endif
    event->accept();
}

void SelectTool::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
    {
        return;
    }

    if (m_currentTool == nullptr)
    {
        return;
    }

    if (m_operation == MoveHandle)
    {
    }
    else
    {
        m_currentTool->mouseReleaseEvent(event);
    }

#if 0
    case MoveHandle:
    {
        m_handle = nullptr;
        break;
    }
#endif
    m_state = HintState;

    updateOperation(event->modifiers());
    setCurrentTool(nullptr);
    event->accept();
}

void SelectTool::keyPressEvent(QKeyEvent *event)
{
    updateOperation(event->modifiers());
    event->accept();
#if 0
    else if (event->key() == Qt::Key_Delete)
    {
        for (QGraphicsItem *item : scene()->selectedItems())
        {
            scene()->removeItem(item);
            delete item;
        }
    }
#endif
}

void SelectTool::keyReleaseEvent(QKeyEvent *event)
{
    updateOperation(event->modifiers());
    event->accept();
}
