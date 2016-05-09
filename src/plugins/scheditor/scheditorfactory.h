#ifndef GRAPHICSEDITORFACTORY_H
#define GRAPHICSEDITORFACTORY_H

#include "core/editormanager/ieditorfactory.h"

class SchEditor;

class SchEditorFactory : public IEditorFactory
{
    Q_OBJECT
public:
    explicit SchEditorFactory(QObject *parent = nullptr);

signals:

public slots:
    void loadSettings();

private:
    QList<SchEditor *> m_editors;

    // IEditorFactory interface
public:
    virtual IEditor *createEditor();
};

#endif // GRAPHICSEDITORFACTORY_H
