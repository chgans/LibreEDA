#ifndef SCHEDITORDOCUMENT_H
#define SCHEDITORDOCUMENT_H

#include "core/editormanager/idocument.h"

#include <QSizeF>
#include <QList>
#include <QUndoStack>

namespace xdl { namespace symbol { class Symbol; } }

class SchItem;
class SchScene;
class SchCommand;

class QUndoStack;

class SchEditorDocument : public IDocument
{
    Q_OBJECT
public:
    explicit SchEditorDocument(QObject *parent = nullptr);

    bool load(QString *errorString, const QString &fileName);

    QList<SchItem *> items(Qt::SortOrder order = Qt::DescendingOrder) const;

    void executeCommand(SchCommand *command);
    QUndoStack *undoStack();

signals:

public slots:

private:
    xdl::symbol::Symbol *m_xdlSymbol;
    SchScene *m_scene;
    QUndoStack *m_commandStack;
    QList<SchItem *> m_items;

    // IDocument interface
public:
    bool save(QString *errorString, const QString &fileName);
    void render(QPainter *painter);
};

#endif // SCHEDITORDOCUMENT_H
