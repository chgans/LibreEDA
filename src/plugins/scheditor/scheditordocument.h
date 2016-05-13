#ifndef SCHEDITORDOCUMENT_H
#define SCHEDITORDOCUMENT_H

#include "core/editormanager/idocument.h"
#include "xdl/symbol.h"

#include <QMap>
#include <QAtomicInteger>

class SchCommand;

// TODO: investigate passing item by const reference and implement cow on item's data?
class SchEditorDocument : public IDocument
{
    Q_OBJECT
public:
    typedef xdl::symbol::Item Item;
    explicit SchEditorDocument(QObject *parent = nullptr);

    bool load(QString *errorString, const QString &fileName);

    QString symbolName() const;
    QString symbolLabel() const;
    const Item *drawingItem(quint64 id) const;
    Item *drawingItem(quint64 id);
    QList<quint64> drawingItemIdList() const;

    quint64 addDrawingItem(Item *item); // takes ownership
    void replaceDrawingItem(quint64 id, Item *item); // takes ownership
    void removeDrawingItem(quint64 id);
    void updateDrawingItem(quint64
                           id); // FIXME: this is actually use to notify that item have been changed externally

signals:
    void drawingItemAdded(quint64 id, const Item *item);
    void drawingItemChanged(quint64 id, const Item *item);
    void drawingItemRemoved(quint64 id);

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

#endif // SCHEDITORDOCUMENT_H
