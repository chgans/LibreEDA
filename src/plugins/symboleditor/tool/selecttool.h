#pragma once

#include "tool/interactivetool.h"

namespace SymbolEditor
{

    class Handle;
    class ItemPropertyEditor;
    class ObjectInspectorView;
    class ObjectInspectorModel;
    class MoveItemTool;
    class CloneItemTool;
    class DragSelectTool;

    class SelectTool : public InteractiveTool
    {
        Q_OBJECT

    public:
        explicit SelectTool(QObject *parent = nullptr);
        ~SelectTool();

    private:
        enum ToolState
        {
            HintState,
            OperationState
        };
        enum Operation
        {
            DragSelect,
            //ClickSelect,
            MoveItem,
            MoveHandle,
            CloneItem
        };
        ToolState m_state;
        Operation m_operation;
        MoveItemTool *m_moveItemTool;
        CloneItemTool *m_cloneItemTool;
        DragSelectTool *m_dragSelectTool;
        InteractiveTool *m_currentTool;

        Handle *m_handle;
        ObjectInspectorModel *m_objectInspectorModel;
        ObjectInspectorView *m_objectInspectorView;
        ItemPropertyEditor *m_itemPropertyEditor;
        QList<QWidget *> m_defaultTaskWidgets;

        void updateOperation(Qt::KeyboardModifiers modifiers);
        void setOperation(Operation operation);
        void updateCursor();
        void setCurrentTool(InteractiveTool *tool);
        void updateTaskWidgets();

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

}
