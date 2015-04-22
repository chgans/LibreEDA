#include "pcbdocument.h"

PcbDocument::PcbDocument(QObject *parent):
    IDocument(parent)
{

}

PcbDocument::~PcbDocument()
{

}

bool PcbDocument::save(QString *errorString, const QString &fileName)
{
    Q_UNUSED(errorString);
    Q_UNUSED(fileName);
    return false;
}
