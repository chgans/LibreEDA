#include "simpletexteditorfactory.h"
#include "simpletexteditorconstants.h"
#include "simpletexteditor.h"

SimpleTextEditorFactory::SimpleTextEditorFactory(QObject *parent) :
    IEditorFactory(parent)
{
    setId(SIMPLETEXTEDITOR_ID);
    setFileExtension(SIMPLETEXTEDITOR_EXT);
    setDisplayName("SCH editor");
}

IEditor *SimpleTextEditorFactory::createEditor()
{
    return new SimpleTextEditor();
}
