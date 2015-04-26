#include "graphicseditorfactory.h"
#include "graphicseditorconstants.h"
#include "graphicseditor.h"

GraphicsEditorFactory::GraphicsEditorFactory(QObject *parent) :
    IEditorFactory(parent)
{
    setId(SCHEDITOR_ID);
    setFileExtension(SCHEDITOR_EXT);
    setDisplayName("SCH editor");
}

IEditor *GraphicsEditorFactory::createEditor()
{
    return new GraphicsEditor();
}
