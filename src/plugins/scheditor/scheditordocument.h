#ifndef SCHEDITORDOCUMENT_H
#define SCHEDITORDOCUMENT_H

#include "core/editormanager/idocument.h"
#include "xdl/symbol.h"

#include <QMap>

class SchCommand;
class QUndoStack;

class SchEditorDocument : public IDocument
{
    Q_OBJECT
public:
    typedef xdl::symbol::Item Item;
    explicit SchEditorDocument(QObject *parent = nullptr);

    bool load(QString *errorString, const QString &fileName);

    QUndoStack *undoStack();

    QString symbolName() const;
    QString symbolLabel() const;
    const Item *drawingItem(quint64 id) const;
    QList<quint64> drawingItemIdList() const;

signals:
    void itemAdded(quint64 id);
    void itemRemoved(quint64 id);
    void itemChanged(quint64 id);

public slots:
    void executeCommand(SchCommand *command);


private:
    QString m_symbolName;
    QString m_symbolLabel;
    QMap<quint64, Item *> m_drawingItemMap;
    QUndoStack *m_commandStack;

    // IDocument interface
public:
    bool save(QString *errorString, const QString &fileName);
    void render(QPainter *painter);
};

#endif // SCHEDITORDOCUMENT_H
