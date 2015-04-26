#ifndef GRAPHICSEDITORFACTORY_H
#define GRAPHICSEDITORFACTORY_H

#include "core/editormanager/ieditorfactory.h"

class SchEditorFactory : public IEditorFactory
{
    Q_OBJECT
public:
    explicit SchEditorFactory(QObject *parent = 0);

signals:

public slots:

    // IEditorFactory interface
public:
    virtual IEditor *createEditor();
};

#endif // GRAPHICSEDITORFACTORY_H
