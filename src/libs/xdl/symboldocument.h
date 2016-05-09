#ifndef XDLSYMBOLDOCUMENT_H
#define XDLSYMBOLDOCUMENT_H

#include "xdl/xdl_global.h"
#include "xdl/symbolitem.h"

#incluse <QPointF>
#include <QList>
#include <QMap>

namespace xdl
{
namespace symbol
{

struct Document
{
public:
    Document();

    QList<uint64_t> items() const;
    Item *item(uint64_t id) const;
    QPointF origin() const;

signals:
    void itemAdded(uint64_t id);
    void itemRemoved(uint64_t id);
    void itemChanged(uint64_t id);
    void originChanged();

public slots:
    uint64_t addItem(Item *data);
    void removeItem(uint64_t id);
    //void updateItem(const Item &data);
    void setOrigin(const QPointF &origin);

protected:
    uint64_t m_itemIndex;
    QMap<uint64_t, Item*> m_items;
    QPointF m_origin;
};

}
}
#endif // XDLSYMBOLDOCUMENT_H
