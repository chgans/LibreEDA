#ifndef GRAPHICSEDITORFACTORY_H
#define GRAPHICSEDITORFACTORY_H

#include "core/editormanager/ieditorfactory.h"

class GraphicsEditorFactory : public IEditorFactory
{
    Q_OBJECT
public:
    explicit GraphicsEditorFactory(QObject *parent = 0);

signals:

public slots:

    // IEditorFactory interface
public:
    virtual IEditor *createEditor();
};

#endif // GRAPHICSEDITORFACTORY_H
