#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QGraphicsScene>
#include "scheditordocument.h"

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
        void addDocumentItem(quint64 id, const SchEditorDocument::Item *item);
        void updateDocumentItem(quint64 id, const SchEditorDocument::Item *item);
        void removeDocumentItem(quint64 id);

    private:
        QMap<quint64, Item *> m_itemMap;
    };

}

#endif // GRAPHICSSCENE_H
