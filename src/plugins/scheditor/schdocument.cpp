#include "schdocument.h"

SchDocument::SchDocument(QObject *parent) :
    IDocument(parent)
{
}

bool SchDocument::load(QString *errorString, const QString &fileName)
{
    Q_UNUSED(errorString);
    Q_UNUSED(fileName);
    return true;
}


bool SchDocument::save(QString *errorString, const QString &fileName)
{
    Q_UNUSED(errorString);
    Q_UNUSED(fileName);
    return true;
}
