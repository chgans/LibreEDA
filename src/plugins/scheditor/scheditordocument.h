#ifndef SCHEDITORDOCUMENT_H
#define SCHEDITORDOCUMENT_H

#include "core/editormanager/idocument.h"

namespace xdl { namespace symbol { class Symbol; } }

class SchCommand;
class QUndoStack;

class SchEditorDocument : public IDocument
{
    Q_OBJECT
public:
    explicit SchEditorDocument(QObject *parent = nullptr);

    bool load(QString *errorString, const QString &fileName);

    QUndoStack *undoStack();

signals:
    void itemAdded();
    void itemRemoved();
    void itemChanged();

public slots:
    void executeCommand(SchCommand *command);


private:
    xdl::symbol::Symbol *m_xdlSymbol;
    QUndoStack *m_commandStack;

    // IDocument interface
public:
    bool save(QString *errorString, const QString &fileName);
    void render(QPainter *painter);
};

#endif // SCHEDITORDOCUMENT_H
