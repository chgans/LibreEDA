#include "clonecommand.h"
#include "document.h"

using namespace SymbolEditor;

CloneCommand::CloneCommand(UndoCommand *parent):
    UndoCommand(parent)
{

}

void CloneCommand::undo()
{
    for (quint64 id : cloneIdList)
    {
        auto clone = document()->drawingItem(id);
        if (clone == nullptr)
        {
            warnItemNotFound("Clone", id);
            continue;
        }
        document()->removeDrawingItem(id);
    }
}

void CloneCommand::redo()
{
    cloneIdList.clear();

    for (quint64 id : itemIdList)
    {
        auto item = document()->drawingItem(id);
        if (item == nullptr)
        {
            warnItemNotFound("Clone", id);
            continue;
        }
        auto clone = item->clone();
        quint64 cloneId = document()->addDrawingItem(clone);
        clone->position += translation;
        document()->updateDrawingItem(cloneId);
        cloneIdList.append(cloneId);
    }

    setText(QString("Clone %1 item").arg(itemIdList.count()));
    if (itemIdList.count() > 1)
    {
        setText(text() + "s");
    }
}
