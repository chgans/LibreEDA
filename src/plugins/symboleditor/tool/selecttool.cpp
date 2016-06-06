#include "tool/selecttool.h"
#include "tool/moveitemtool.h"
#include "tool/cloneitemtool.h"
#include "tool/dragselecttool.h"

#include "view/scene.h"
#include "view/view.h"
#include "item/item.h"
#include "handle/handle.h"

#include "propertyeditor/itempropertyeditor.h"
#include "objectinspector/objectinspectorview.h"
#include "objectinspector/objectinspectormodel.h"
#include "objectinspector/iconitemdelegate.h"

#include "command/command.h"

#include <QMouseEvent>
#include <QKeyEvent>
#include <QAction>
#include <QHeaderView>

using namespace SymbolEditor;

SelectTool::SelectTool(QObject *parent):
    InteractiveTool(parent)
{
    QAction *action = new QAction(QIcon::fromTheme("edit-select"),
                                  "Select <i>Esc</i>", nullptr);
    action->setShortcut(QKeySequence(Qt::Key_Escape));
    setAction(action);
    setToolGroup("interactive-tools");

    setupObjectInspector();
    setupPropertyBrowser();
    setupSubTools();
    initStateMachine();
}

SelectTool::~SelectTool()
{

}

void SelectTool::cancel()
{
}

void SelectTool::activate(View *view)
{
    setView(view);
    initStateMachine();
}

void SelectTool::desactivate()
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
        auto items = scene()->selectedObjects();
        if (!items.isEmpty())
        {
            m_itemPropertyEditor->setItem(items.first());
        }
        return;
    }

    if (m_operation == MoveHandle)
    {
    }
    else
    {
        m_currentTool->mouseReleaseEvent(event);

        auto items = scene()->selectedObjects();
        if (!items.isEmpty())
        {
            m_itemPropertyEditor->setItem(items.first());
        }
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
        m_currentTool->desactivate();
    }

    m_currentTool = tool;

    if (m_currentTool != nullptr)
    {
        m_currentTool->activate(view());
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

void SelectTool::setupObjectInspector()
{
    m_objectInspectorModel = new ObjectInspectorModel(this);
    m_objectInspectorView = new ObjectInspectorView;
    m_defaultTaskWidgets << m_objectInspectorView;

    // Fake data for now
    m_objectInspectorModel->addTopLevelItem(12, "Bezier Curve", QIcon::fromTheme("draw-bezier-curves"));
    m_objectInspectorModel->addTopLevelItem(16, "Rectangle", QIcon::fromTheme("draw-rectangle"));
    m_objectInspectorModel->addTopLevelItem(18, "Group", QIcon::fromTheme("object-group"));
    m_objectInspectorModel->addChildItem(18, 28, "Rectangle", QIcon::fromTheme("draw-rectangle"));
    m_objectInspectorModel->addChildItem(18, 29, "Label", QIcon::fromTheme("draw-text"));
    m_objectInspectorModel->addChildItem(18, 31, "Line", QIcon::fromTheme("draw-line"));
    m_objectInspectorModel->addChildItem(18, 45, "Line", QIcon::fromTheme("draw-line"));

    // Setup view
    m_objectInspectorView->setModel(m_objectInspectorModel);
    m_objectInspectorView->header()->setStretchLastSection(false);
    m_objectInspectorView->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    m_objectInspectorView->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    m_objectInspectorView->header()->setSectionResizeMode(2, QHeaderView::ResizeToContents);

    auto visibilityDelagate =  new IconItemDelegate(this);
    visibilityDelagate->activeIconName = "object-visible";
    visibilityDelagate->inactiveIconName = "object-hidden";
    m_objectInspectorView->setItemDelegateForColumn(1, visibilityDelagate);

    auto lockDelagate =  new IconItemDelegate(this);
    lockDelagate->activeIconName = "object-locked";
    lockDelagate->inactiveIconName = "object-unlocked";
    m_objectInspectorView->setItemDelegateForColumn(2, lockDelagate);
}

void SelectTool::setupPropertyBrowser()
{
    m_itemPropertyEditor = new ItemPropertyEditor;
    m_defaultTaskWidgets << m_itemPropertyEditor;
}

void SelectTool::setupSubTools()
{
    m_moveItemTool = new MoveItemTool(this);
    connect(m_moveItemTool, &Tool::commandRequested,
            this, &Tool::commandRequested);

    m_cloneItemTool = new CloneItemTool(this);
    connect(m_cloneItemTool, &Tool::commandRequested,
            this, &Tool::commandRequested);

    m_dragSelectTool = new DragSelectTool(this);
}

void SelectTool::initStateMachine()
{
    m_state = HintState;
    m_operation = DragSelect;
    m_handle = nullptr;
    m_currentTool = nullptr;
    m_taskWidgets = m_defaultTaskWidgets;
}
