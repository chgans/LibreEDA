#include "clonecommand.h"
#include "document.h"

namespace SymbolEditor
{

    CloneCommand::CloneCommand(UndoCommand *parent):
        UndoCommand(parent)
    {

    }

    void CloneCommand::undo()
    {
        for (quint64 id : cloneIdList)
        {
            auto clone = document()->item(id);
            if (clone == nullptr)
            {
                warnItemNotFound("Clone", id);
                continue;
            }
            document()->removeItem(id);
        }
    }

    void CloneCommand::redo()
    {
        cloneIdList.clear();

        for (quint64 id : itemIdList)
        {
            auto item = document()->item(id);
            quint64 cloneId = document()->cloneItem(id);
            auto newPosition = item->position() + translation;
            document()->setItemProperty(cloneId, xdl::symbol::Item::PositionProperty, newPosition);
            cloneIdList.append(cloneId);
        }

        setText(QString("Clone %1 item").arg(itemIdList.count()));
        if (itemIdList.count() > 1)
        {
            setText(text() + "s");
        }
    }

}
