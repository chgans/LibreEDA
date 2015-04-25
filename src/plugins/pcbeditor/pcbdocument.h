#ifndef PCBDOCUMENT_H
#define PCBDOCUMENT_H

#include "core/editormanager/idocument.h"

class DesignLayer;
class GraphicsItem;

class PcbDocument : public IDocument
{
public:
    PcbDocument(QObject *parent = nullptr);
    ~PcbDocument();

    bool load(QString *errorString, const QString &fileName);

    QList<DesignLayer *> layers() const;
    QList<GraphicsItem *> items() const;

private:
    QList<DesignLayer *> m_layers;
    QList<GraphicsItem *> m_items;

    GraphicsItem *parseItem;
    DesignLayer *mparseLayer;
    int parseLayerIndex();
    QPointF parsePoint();
    QLineF parseLine();

    // IDocument interface
public:
    bool save(QString *errorString, const QString &fileName);
};

#endif // PCBDOCUMENT_H
