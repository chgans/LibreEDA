#pragma once

#include "tool/interactivetool.h"
#include "document.h"
#include "view/palette.h"

#include <QItemSelection>


namespace SymbolEditor
{

    class ItemPropertyEditor;
    class ObjectInspectorView;
    class ObjectInspectorModel;

    class SelectTool : public InteractiveTool
    {
        Q_OBJECT

    public:
        explicit SelectTool(Editor *editor);
        ~SelectTool();

        void setPalette(Palette palette);
        Palette palette() const;

    public slots:
        void addDocumentItem(quint64 id, const Document::Item *item);
        void updateDocumentItem(quint64 id, const Document::Item *item);
        void updateDocumentItemProperty(quint64 itemId, quint64 propertyId, const QVariant &value);
        void removeDocumentItem(quint64 id);

    private slots:
        void onObjectInspectorVisibilityChangeRequested(quint64 id, bool visibility);
        void onObjectInspectorLockStateChangeRequested(quint64 id, bool lockState);
        void onObjectInspectorSelectionChanged(const QItemSelection &selected,
                                               const QItemSelection &deselected);
        void onSceneSelectionChanged();

    private:
        void setupObjectInspector();
        void setupPropertyBrowser();
        void setupSubTools();
        void initStateMachine();

        Palette m_palette;

        // TODO: Move these 2 to ObjectInspectorWidget (SymbolEditor specific)
        ObjectInspectorModel *m_objectInspectorModel;
        ObjectInspectorView *m_objectInspectorView;
        ItemPropertyEditor *m_propertyEditor;

        bool m_changingSelection = false;

        // GraphicsTool interface
    public slots:
        virtual void cancel();

        // GraphicsTool interface
    public:
        virtual void activate();
        virtual void desactivate();
        virtual void mousePressEvent(QMouseEvent *event);
        virtual void mouseMoveEvent(QMouseEvent *event);
        virtual void mouseReleaseEvent(QMouseEvent *event);
        virtual void keyPressEvent(QKeyEvent *event);
        virtual void keyReleaseEvent(QKeyEvent *event);
    };

}
