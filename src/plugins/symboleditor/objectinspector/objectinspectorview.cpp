#include "objectinspectorview.h"
#include "iconitemdelegate.h"

#include <QHeaderView>

namespace SymbolEditor
{

    ObjectInspectorView::ObjectInspectorView(QWidget *parent):
        QTreeView(parent)
    {
        setSelectionMode(QTreeView::ExtendedSelection);
        setSelectionBehavior(QTreeView::SelectRows);

        m_visibilityItemDelegate =  new IconItemDelegate(this);
        m_visibilityItemDelegate->setCheckedIcon(QIcon::fromTheme("object-visible"));
        m_visibilityItemDelegate->setUncheckedIcon(QIcon::fromTheme("object-hidden"));

        m_lockStateItemDelegate =  new IconItemDelegate(this);
        m_lockStateItemDelegate->setCheckedIcon(QIcon::fromTheme("object-locked"));
        m_lockStateItemDelegate->setUncheckedIcon(QIcon::fromTheme("object-unlocked"));
    }

    void ObjectInspectorView::setModel(QAbstractItemModel *model)
    {
        QTreeView::setModel(model);

        header()->setStretchLastSection(false);
        header()->setSectionResizeMode(0, QHeaderView::Stretch);
        header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
        header()->setSectionResizeMode(2, QHeaderView::ResizeToContents);

        setItemDelegateForColumn(1, m_visibilityItemDelegate);
        setItemDelegateForColumn(2, m_lockStateItemDelegate);
    }

}
