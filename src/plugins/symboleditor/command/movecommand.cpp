#include "movecommand.h"
#include "document.h"

using namespace SymbolEditor;

TranslateCommand::TranslateCommand(UndoCommand *parent):
    UndoCommand(parent)
{
    setText("Move ? item(s)");
}

void TranslateCommand::undo()
{
    for (quint64 id : itemIdList)
    {
        auto item = document()->drawingItem(id);
        if (item == nullptr)
        {
            warnItemNotFound("Translate", id);
            continue;
        }
        item->position -= amount;
        document()->updateDrawingItem(id);
    }
}

void TranslateCommand::redo()
{
    for (quint64 id : itemIdList)
    {
        auto item = document()->drawingItem(id);
        if (item == nullptr)
        {
            warnItemNotFound("Translate", id);
            continue;
        }
        item->position += amount;
        document()->updateDrawingItem(id);
    }

    setText(QString("Move %1 item").arg(itemIdList.count()));
    if (itemIdList.count() > 1)
    {
        setText(text() + "s");
    }
}
