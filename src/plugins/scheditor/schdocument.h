#ifndef GRAPHICSDOCUMENT_H
#define GRAPHICSDOCUMENT_H

#include "core/editormanager/idocument.h"

class SchDocument : public IDocument
{
    Q_OBJECT
public:
    explicit SchDocument(QObject *parent = 0);

    bool load(QString *errorString, const QString &fileName);

signals:

public slots:


    // IDocument interface
public:
    bool save(QString *errorString, const QString &fileName);
};

#endif // GRAPHICSDOCUMENT_H
