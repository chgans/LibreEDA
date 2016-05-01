#ifndef SCHDOCUMENTGRAPHICSOBJECT_H
#define SCHDOCUMENTGRAPHICSOBJECT_H

#include "schdocumentobject.h"

#include <QPen>
#include <QBrush>

class SchDocumentGraphicsObject : public SchDocumentObject
{
    Q_GADGET
public:
    explicit SchDocumentGraphicsObject(SchDocumentObject *parent = nullptr);
};


#endif // SCHDOCUMENTGRAPHICSOBJECT_H
