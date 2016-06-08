#include "objectinspectormodel.h"

namespace SymbolEditor
{

    ObjectInspectorModel::ObjectInspectorModel(QObject *parent)
        : QAbstractItemModel(parent),
          m_rootItem(new ObjectInspectorItem)
    {
    }

    QVariant ObjectInspectorModel::headerData(int section, Qt::Orientation orientation, int role) const
    {
        if (orientation != Qt::Horizontal)
        {
            return QVariant();
        }

        if (role != Qt::DisplayRole)
        {
            return QVariant();
        }

        switch (section)
        {
            case 0:
                return "Object";
            case 1:
                return "V";
            case 2:
                return "L";
            default:
                return QVariant();
        }
    }

    QModelIndex ObjectInspectorModel::index(int row, int column, const QModelIndex &parent) const
    {
        if (parent.isValid() && parent.column() != 0)
        {
            return QModelIndex();
        }

        auto parentItem = itemFromModelIndex(parent);
        auto childItem = parentItem->child(row);
        if (childItem != nullptr)
        {
            return createIndex(row, column, childItem);
        }
        else
        {
            return QModelIndex();
        }
    }

    QModelIndex ObjectInspectorModel::parent(const QModelIndex &index) const
    {
        if (!index.isValid())
        {
            return QModelIndex();
        }

        auto *childItem = itemFromModelIndex(index);
        auto *parentItem = childItem->parent();

        if (parentItem == m_rootItem.data())
        {
            return QModelIndex();
        }

        return createIndex(parentItem->IndexInParent(), 0, parentItem);
    }

    int ObjectInspectorModel::rowCount(const QModelIndex &parent) const
    {
        if (parent.column() > 0)
        {
            return 0;
        }

        auto parentItem = itemFromModelIndex(parent);
        return parentItem->childCount();
    }

    int ObjectInspectorModel::columnCount(const QModelIndex &parent) const
    {
        Q_UNUSED(parent);
        return 3;
    }

    void ObjectInspectorModel::addTopLevelItem(quint64 id, const QString &text, const QIcon &icon)
    {
        int lastRow = m_rootItem->childCount();
        auto rootIndex = QModelIndex();
        beginInsertRows(rootIndex, lastRow, lastRow);
        createItem(m_rootItem.data(), id, text, icon);
        endInsertRows();
    }

    void ObjectInspectorModel::addChildItem(quint64 parentId, quint64 id, const QString &text, const QIcon &icon)
    {
        auto parent = m_allItems.value(parentId);
        int lastRow = parent->childCount();
        auto parentIndex = createIndex(parent->IndexInParent(), 0, parent);
        beginInsertRows(parentIndex, lastRow, lastRow);
        createItem(parent, id, text, icon);
        endInsertRows();
    }

    void ObjectInspectorModel::removeItem(quint64 id)
    {
        auto item = m_allItems.value(id);
        int row = item->IndexInParent();
        auto parent = item->parent();
        auto parentIndex = createIndex(row, 0, parent);

        beginRemoveRows(parentIndex, row, row);
        parent->removeChild(item);
        delete item;
        endRemoveRows();
    }

    void ObjectInspectorModel::setItemVisibility(quint64 id, bool visible)
    {
        auto item = m_allItems.value(id);

        if (item->m_visible == visible)
        {
            return;
        }
        item->m_visible = visible;

        int row = 0;
        int column = 1;
        auto index = createIndex(row, column, item);
        emit dataChanged(index, index);
    }

    void ObjectInspectorModel::setItemLockState(quint64 id, bool locked)
    {
        auto item = m_allItems.value(id);

        if (item->m_locked == locked)
        {
            return;
        }
        item->m_locked = locked;

        int row = 0;
        int column = 2;
        auto index = createIndex(row, column, item);
        emit dataChanged(index, index);
    }

    QModelIndex ObjectInspectorModel::indexForDocumentId(quint64 id)
    {
        auto item = m_allItems.value(id);
        int row = item->IndexInParent();
        int column = 0;
        return createIndex(row, column, item);
    }

    quint64 ObjectInspectorModel::documentIdForIndex(const QModelIndex &index)
    {
        return m_allItems.key(itemFromModelIndex(index));
    }

    ObjectInspectorItem *ObjectInspectorModel::itemFromModelIndex(const QModelIndex &index) const
    {
        if (!index.isValid())
        {
            return m_rootItem.data();
        }
        else
        {
            return static_cast<ObjectInspectorItem*>(index.internalPointer());
        }
    }

    ObjectInspectorItem *ObjectInspectorModel::createItem(ObjectInspectorItem *parent,
                                                          quint64 id, const QString &text, QIcon icon)
    {
        auto item = new ObjectInspectorItem(parent);
        item->m_displayData = QString("%1 %2").arg(text).arg(id);
        item->m_decorationData = icon;
        item->m_visible = true;
        item->m_locked = true;
        parent->appendChild(item);
        m_allItems.insert(id, item);
        return item;
    }

    QVariant ObjectInspectorModel::data(const QModelIndex &index, int role) const
    {
        if (!index.isValid())
            return QVariant();

        switch (role)
        {
            case Qt::DisplayRole:
                switch (index.column())
                {
                    case 0:
                        return itemFromModelIndex(index)->m_displayData;
                    case 1:
                        return itemFromModelIndex(index)->m_visible;
                    case 2:
                        return itemFromModelIndex(index)->m_locked;
                    default:
                        return QVariant();
                }
            case Qt::DecorationRole:
                switch (index.column())
                {
                    case 0:
                        return itemFromModelIndex(index)->m_decorationData;
                    default:
                        return QVariant();
                }
            default:
                return QVariant();
        }
    }

    bool ObjectInspectorModel::setData(const QModelIndex &index, const QVariant &value, int role)
    {
        if (!index.isValid())
        {
            return false;
        }

        if (index.column() == 0 || index.column() >= 3)
        {
            return false;
        }

        if (role != Qt::EditRole)
        {
            return false;
        }

        if (!value.canConvert<bool>())
        {
            return false;
        }

        auto id = documentIdForIndex(index);
        auto boolValue = value.toBool();
        if (index.column() == 1)
        {
            emit itemVisiblityChangeRequested(id, boolValue);
        }
        else
        {
            emit itemLockStateChangeRequested(id, boolValue);
        }
        return true;
    }

}
