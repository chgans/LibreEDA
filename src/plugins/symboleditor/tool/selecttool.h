#pragma once

#include "tool/interactivetool.h"
#include "document.h"

#include <QItemSelection>

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

    public slots:
        void addDocumentItem(quint64 id, const Document::Item *item);
        void updateDocumentItem(quint64 id, const Document::Item *item);
        void removeDocumentItem(quint64 id);

    private slots:
        void onObjectInspectorSelectionChanged(const QItemSelection &selected,
                                               const QItemSelection &deselected);
        void onSceneSelectionChanged();

    private:
        void setupObjectInspector();
        void setupPropertyBrowser();
        void setupSubTools();
        void initStateMachine();

        ObjectInspectorModel *m_objectInspectorModel;
        ObjectInspectorView *m_objectInspectorView;
        ItemPropertyEditor *m_itemPropertyEditor;
        bool m_changingSelection = false;

        // GraphicsTool interface
    public slots:
        virtual void cancel();

        // GraphicsTool interface
    public:
        virtual void activate(View *view);
        virtual void desactivate();
        virtual void mousePressEvent(QMouseEvent *event);
        virtual void mouseMoveEvent(QMouseEvent *event);
        virtual void mouseReleaseEvent(QMouseEvent *event);
        virtual void keyPressEvent(QKeyEvent *event);
        virtual void keyReleaseEvent(QKeyEvent *event);
    };

}
