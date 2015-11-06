#ifndef SIMPLETEXTDOCUMENT_H
#define SIMPLETEXTDOCUMENT_H

#include "core/editormanager/idocument.h"

class QTextDocument;

class SimpleTextDocument : public IDocument
{
public:
    SimpleTextDocument();
    bool load(QString *errorString, const QString &fileName);

    QTextDocument *textDocument();

    // IDocument interface
public:
    bool save(QString *errorString, const QString &fileName);

private:
    QTextDocument *m_textDocument;
};

#endif // SIMPLETEXTDOCUMENT_H
