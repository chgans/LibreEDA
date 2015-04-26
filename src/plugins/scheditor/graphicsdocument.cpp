#include "graphicsdocument.h"

GraphicsDocument::GraphicsDocument(QObject *parent) :
    IDocument(parent)
{
}

bool GraphicsDocument::load(QString *errorString, const QString &fileName)
{
    Q_UNUSED(errorString);
    Q_UNUSED(fileName);
    return true;
}


bool GraphicsDocument::save(QString *errorString, const QString &fileName)
{
    Q_UNUSED(errorString);
    Q_UNUSED(fileName);
    return true;
}
