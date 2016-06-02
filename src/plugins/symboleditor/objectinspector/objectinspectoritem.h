#pragma once

#include <QString>
#include <QIcon>
#include <QScopedArrayPointer>

namespace SymbolEditor
{

    class ObjectInspectorItem
    {
    public:
        ObjectInspectorItem(ObjectInspectorItem *parent = nullptr);
        ~ObjectInspectorItem();

        // Access to children
        int childCount() const;
        ObjectInspectorItem *child(int index) const;
        void appendChild(ObjectInspectorItem *child);
        void removeChild(ObjectInspectorItem *child);

        // Access to parent
        int IndexInParent() const;
        ObjectInspectorItem *parent() const;

        // Document item specific (to  be rendered in columns)
        bool m_visible;
        bool m_locked;

        // IObjectInspectorItemFactory's job here (with a quint64 documentTypeId parameter):
        QIcon m_decorationData;
        QString m_displayData;
        QString m_toolTipData;

    private:
        ObjectInspectorItem * m_parent;
        QList<ObjectInspectorItem *> m_children;
    };

}
