#include "simpletexteditorfactory.h"
#include "simpletexteditorconstants.h"
#include "simpletexteditor.h"

SimpleTextEditorFactory::SimpleTextEditorFactory(QObject *parent) :
    IEditorFactory(parent)
{
    setId(SIMPLETEXTEDITOR_ID);
    setFileExtension(SIMPLETEXTEDITOR_EXT);
    setDisplayName("Simple text editor");
}

IEditor *SimpleTextEditorFactory::createEditor()
{
    return new SimpleTextEditor();
}
