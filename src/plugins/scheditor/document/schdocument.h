#ifndef SYMBOLDOCUMENT_H
#define SYMBOLDOCUMENT_H

#include "document/itemdata.h"

#include <QObject>
#include <QMap>

class SymbolDocument: public QObject
{
    Q_OBJECT

public:
    explicit SymbolDocument(QObject *parent = nullptr);

    QList<uint64_t> items() const;
    ItemData *item(uint64_t id) const;
    QPointF origin() const;

signals:
    void itemAdded(uint64_t id);
    void itemRemoved(uint64_t id);
    void itemChanged(uint64_t id);
    void originChanged();

public slots:
    uint64_t addItem(ItemData *data);
    void removeItem(uint64_t id);
    //void updateItem(const ItemData &data);
    void setOrigin(const QPointF &origin);

protected:
    uint64_t m_itemIndex;
    QMap<uint64_t, ItemData*> m_items;
    QPointF m_origin;
};

#endif // SYMBOLDOCUMENT_H
