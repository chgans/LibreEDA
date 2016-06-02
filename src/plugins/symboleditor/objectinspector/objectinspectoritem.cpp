#include "objectinspectoritem.h"

namespace SymbolEditor
{

    ObjectInspectorItem::ObjectInspectorItem(ObjectInspectorItem *parent):
        m_parent(parent)
    {

    }

    ObjectInspectorItem::~ObjectInspectorItem()
    {
        qDeleteAll(m_children);
    }

    int ObjectInspectorItem::childCount() const
    {
        return m_children.count();
    }

    ObjectInspectorItem *ObjectInspectorItem::child(int index) const
    {
        return m_children.value(index);
    }

    void ObjectInspectorItem::appendChild(ObjectInspectorItem *child)
    {
        m_children.append(child);
    }

    void ObjectInspectorItem::removeChild(ObjectInspectorItem *child)
    {
        m_children.removeOne(child);
    }

    int ObjectInspectorItem::IndexInParent() const
    {
        if (m_parent != nullptr)
        {
            return m_parent->m_children.indexOf(const_cast<ObjectInspectorItem*>(this));
        }
        return 0;
    }

    ObjectInspectorItem *ObjectInspectorItem::parent() const
    {
        return m_parent;
    }


}
