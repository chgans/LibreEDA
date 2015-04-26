#ifndef GRAPHICSDOCUMENT_H
#define GRAPHICSDOCUMENT_H

#include "core/editormanager/idocument.h"

#include <QList>

class SchItem;

class SchDocument : public IDocument
{
    Q_OBJECT
public:
    explicit SchDocument(QObject *parent = 0);

    bool load(QString *errorString, const QString &fileName);

    QList<SchItem *> items() const;

signals:

public slots:

private:
    QList<SchItem *> m_items;

    // IDocument interface
public:
    bool save(QString *errorString, const QString &fileName);
};

#endif // GRAPHICSDOCUMENT_H
