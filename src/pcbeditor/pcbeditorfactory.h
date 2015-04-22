#ifndef PCBEDITORFACTORY_H
#define PCBEDITORFACTORY_H

#include "core/editormanager/ieditorfactory.h"

class PcbEditorFactory : public IEditorFactory
{
    Q_OBJECT

public:
    PcbEditorFactory(QObject *parent = 0);
    ~PcbEditorFactory();

    // IEditorFactory interface
public:
    virtual IEditor *createEditor();
};

#endif // PCBEDITORFACTORY_H
