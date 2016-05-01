#ifndef SCHDOCUMENTOBJECT_H
#define SCHDOCUMENTOBJECT_H

#include <QObject>

class SchDocumentObject
{
    Q_GADGET

public:
    explicit SchDocumentObject(SchDocumentObject *parent = nullptr);

private:
    SchDocumentObject *m_parent;
};

#endif // SCHDOCUMENTOBJECT_H
