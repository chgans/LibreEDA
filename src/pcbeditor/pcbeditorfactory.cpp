#include "pcbeditorfactory.h"
#include "pcbeditorconstants.h"
#include "pcbeditorwidget.h"
#include "pcbeditor.h"

PcbEditorFactory::PcbEditorFactory(QObject *parent):
    IEditorFactory(parent)
{
    setId(PCBEDITOR_ID);
    setFileExtension(PCBEDITOR_EXT);
    setDisplayName("PCB editor");
}

PcbEditorFactory::~PcbEditorFactory()
{

}



IEditor *PcbEditorFactory::createEditor()
{
    return new PcbEditor();
}
