#pragma once

#include <QTreeView>

namespace SymbolEditor
{

    class IconItemDelegate;

    class ObjectInspectorView: public QTreeView
    {
        Q_OBJECT

    public:
        ObjectInspectorView(QWidget *parent = nullptr);
        void initialise();

    private:
        IconItemDelegate *m_visibilityItemDelegate;
        IconItemDelegate *m_lockStateItemDelegate;

        // QAbstractItemView interface
    public:
        void setModel(QAbstractItemModel *model);
    };

}
