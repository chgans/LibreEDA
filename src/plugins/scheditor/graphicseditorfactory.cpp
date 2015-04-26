#include "graphicseditorfactory.h"
#include "graphicseditorconstants.h"
#include "graphicseditor.h"

SchEditorFactory::SchEditorFactory(QObject *parent) :
    IEditorFactory(parent)
{
    setId(SCHEDITOR_ID);
    setFileExtension(SCHEDITOR_EXT);
    setDisplayName("SCH editor");
}

IEditor *SchEditorFactory::createEditor()
{
    return new SchEditor();
}
