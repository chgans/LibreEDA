#ifndef GRAPHICSSCENEDATA_H
#define GRAPHICSSCENEDATA_H

#include "document/schdocumentobject.h"

#include <QSize>

class GraphicsSceneData : public SchDocumentObject
{
    Q_GADGET

    Q_PROPERTY(QSize size READ size WRITE setSize)

    QSize m_size;

public:
    GraphicsSceneData(SchDocumentObject *parent = nullptr);

    QSize size() const
    {
        return m_size;
    }

public slots:
    void setSize(QSize size)
    {
        m_size = size;
    }
};

#endif // GRAPHICSSCENEDATA_H
