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

    public slots:
        void applySettings(const Settings &settings);
        void addDocumentItem(quint64 id, const Document::Item *item);
        void updateDocumentItem(quint64 id, const Document::Item *item);
        void removeDocumentItem(quint64 id);

    private:
        QMap<quint64, Item *> m_itemMap;
    };

}
