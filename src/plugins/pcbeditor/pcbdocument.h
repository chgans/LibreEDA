#ifndef PCBDOCUMENT_H
#define PCBDOCUMENT_H

#include "core/editormanager/idocument.h"

#include <QSizeF>

class DesignLayer;
class GraphicsItem;

class PcbDocument : public IDocument
{
public:
    PcbDocument(QObject *parent = nullptr);
    ~PcbDocument();

    bool load(QString *errorString, const QString &fileName);

    QSizeF boardSize() const;
    QList<int> layerStack() const;
    QList<GraphicsItem *> items() const;

private:
    QSizeF m_boardSize;
    QList<int> m_layerStack;
    QList<GraphicsItem *> m_items;

    // IDocument interface
public:
    bool save(QString *errorString, const QString &fileName);
};

#endif // PCBDOCUMENT_H
