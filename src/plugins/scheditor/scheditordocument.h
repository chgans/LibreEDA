#ifndef SCHEDITORDOCUMENT_H
#define SCHEDITORDOCUMENT_H

#include "core/editormanager/idocument.h"

#include <QSizeF>
#include <QList>
#include <QUndoStack>

class SchItem;
class SchScene;
class SchCommand;

class QUndoStack;

class SchEditorDocument : public IDocument
{
    Q_OBJECT
public:
    explicit SchEditorDocument(QObject *parent = 0);

    bool load(QString *errorString, const QString &fileName);

    // Page template
    QSizeF pageSize() const;
    void setPageSize(const QSizeF &size);

    QList<SchItem *> items(Qt::SortOrder order = Qt::DescendingOrder) const;

    void executeCommand(SchCommand *command);
    QUndoStack *undoStack();

signals:

public slots:

private:
    SchScene *m_scene;
    QUndoStack *m_commandStack;
    QList<SchItem *> m_items;

    // IDocument interface
public:
    bool save(QString *errorString, const QString &fileName);
};

#endif // SCHEDITORDOCUMENT_H
