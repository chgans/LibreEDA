#include "movecommand.h"
#include "document.h"

namespace SymbolEditor
{

    TranslateCommand::TranslateCommand(UndoCommand *parent):
        UndoCommand(parent)
    {
        setText("Move ? item(s)");
    }

    void TranslateCommand::undo()
    {
        for (quint64 id : itemIdList)
        {
            auto item = document()->item(id);
            if (item == nullptr)
            {
                warnItemNotFound("Translate", id);
                continue;
            }
            auto newPosition = item->position() - amount;
            document()->setItemProperty(id, xdl::symbol::Item::PositionProperty, newPosition);
        }
    }

    void TranslateCommand::redo()
    {
        for (quint64 id : itemIdList)
        {
            auto item = document()->item(id);
            if (item == nullptr)
            {
                warnItemNotFound("Translate", id);
                continue;
            }
            auto newPosition = item->position() + amount;
            document()->setItemProperty(id, xdl::symbol::Item::PositionProperty, newPosition);
        }

        setText(QString("Move %1 item").arg(itemIdList.count()));
        if (itemIdList.count() > 1)
        {
            setText(text() + "s");
        }
    }

}
