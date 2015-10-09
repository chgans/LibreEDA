#ifndef GRAPHICSDOCUMENT_H
#define GRAPHICSDOCUMENT_H

#include "document/schdocumentobject.h"

#include <QObject>
#include <QMap>
#include <QUuid>
#include <QSizeF>


class SchSceneData;

class SchDocument: public QObject
{
    Q_OBJECT


public:
    explicit SchDocument(QObject *parent = 0);


    const SchDocumentObject *documentObject(const QUuid &uid) const;

    const SchSceneData *sceneData() const;

signals:

public slots:

private:
    QMap<QUuid, SchDocumentObject*> m_objects;

};

#endif // GRAPHICSDOCUMENT_H
