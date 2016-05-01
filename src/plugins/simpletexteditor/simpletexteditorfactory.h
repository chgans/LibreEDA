#ifndef SIMPLETEXTEDITORFACTORY_H
#define SIMPLETEXTEDITORFACTORY_H

#include "core/editormanager/ieditorfactory.h"

class SimpleTextEditorFactory : public IEditorFactory
{
    Q_OBJECT
public:
    explicit SimpleTextEditorFactory(QObject *parent = nullptr);

signals:

public slots:

    // IEditorFactory interface
public:
    virtual IEditor *createEditor();
};

#endif // SIMPLETEXTEDITORFACTORY_H
