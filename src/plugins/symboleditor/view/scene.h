#pragma once

#include <QGraphicsScene>
#include "document.h"

namespace SymbolEditor
{

    class Item;
    class Settings;

    class Scene : public QGraphicsScene
    {
        Q_OBJECT

    public:
        explicit Scene(QObject *parent = nullptr);
        ~Scene();

        QList<Item *> selectedObjects();
        Item *itemForDocumentId(quint64 id) const;

    public slots:
        void applySettings(const Settings &settings);
        void addDocumentItem(quint64 id, const Document::Item *item);
        void updateDocumentItem(quint64 id, const Document::Item *item);
        void updateDocumentItemProperty(quint64 itemId, quint64 propertyId, const QVariant &value);
        void removeDocumentItem(quint64 id);

    private:
        QMap<quint64, Item *> m_itemMap;
        QPen makePen(const Document::Item *item);
        QBrush makeBrush(const Document::Item *item);
    };

}
