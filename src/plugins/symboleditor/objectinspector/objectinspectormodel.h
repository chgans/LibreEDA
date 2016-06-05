#pragma once

#include <QAbstractItemModel>
#include <QScopedPointer>
#include "objectinspectoritem.h"

namespace SymbolEditor
{

    class ObjectInspectorModel : public QAbstractItemModel
    {
        Q_OBJECT

    public:
        explicit ObjectInspectorModel(QObject *parent = 0);

        // Bridge to the ObjectInspectorView
        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
        QModelIndex index(int row, int column,
                          const QModelIndex &parent = QModelIndex()) const override;
        QModelIndex parent(const QModelIndex &index) const override;
        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        int columnCount(const QModelIndex &parent = QModelIndex()) const override;


        // Bridge to object holders
        void addTopLevelItem(quint64 id, const QString &text, const QIcon &icon);
        void addChildItem(quint64  parentId, quint64 id, const QString &text, const QIcon &icon);
        void removeItem(quint64 id);
        // TBD: Inspector can change:
        //  - visible and locked
        //  - grouping?

    private:
        ObjectInspectorItem *itemFromModelIndex(const QModelIndex &index) const;
        QScopedPointer<ObjectInspectorItem> m_rootItem;
        QMap<quint64, ObjectInspectorItem*> m_allItems;

        ObjectInspectorItem *createItem(ObjectInspectorItem *parent,
                                        quint64 id, const QString &text, QIcon icon);

        // QAbstractItemModel interface
    public:
        bool setData(const QModelIndex &index, const QVariant &value, int role);
    };

}
