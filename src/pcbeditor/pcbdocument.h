#ifndef PCBDOCUMENT_H
#define PCBDOCUMENT_H

#include "core/editormanager/idocument.h"

class PcbDocument : public IDocument
{
public:
    PcbDocument(QObject *parent = nullptr);
    ~PcbDocument();

    // IDocument interface
public:
    bool save(QString *errorString, const QString &fileName);
};

#endif // PCBDOCUMENT_H
