#pragma once

#include "core/editormanager/idocument.h"
#include "xdl/symbol.h"

#include <QMap>
#include <QAtomicInteger>

namespace SymbolEditor
{

    // TODO: investigate passing item by const reference and implement cow on item's data?
    class Document : public IDocument
    {
        Q_OBJECT
    public:
        typedef xdl::symbol::Item Item;
        typedef xdl::symbol::ItemGroup ItemGroup;

        explicit Document(QObject *parent = nullptr);

        bool load(QString *errorString, const QString &fileName);

        QString symbolName() const;
        QString symbolLabel() const;
        const Item *item(quint64 id) const;
        Item *item(quint64 id);
        QList<quint64> itemIdList() const;

        quint64 addItem(Item *item); // takes ownership
        void replaceItem(quint64 id, Item *item); // takes ownership
        void removeItem(quint64 id);
        void updateItem(quint64
                               id); // FIXME: this is actually use to notify that item have been changed externally
        void setItemProperty(quint64 id, const QString &name, const QVariant &value);

    signals:
        void itemAdded(quint64 id, const Item *item);
        void itemChanged(quint64 id, const Item *item);
        void itemRemoved(quint64 id);

        void itemGroupCreated(quint64 id, const ItemGroup *group);
        void itemGroupDestroyed(quint64 id, const ItemGroup *group); // TBD: children list
        void itemParentChanged(quint64 id, const Item *item); /// TBD: parent changes (not grouping)

    private:
        QAtomicInteger<quint64> m_itemIndex;
        QString m_symbolName;
        QString m_symbolLabel;
        QMap<quint64, Item *> m_drawingItemMap;

        // IDocument interface
    public:
        bool save(QString *errorString, const QString &fileName);
        void render(QPainter *painter);
    };
}
