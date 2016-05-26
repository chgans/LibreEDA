#pragma once

#include "command.h"

namespace SymbolEditor
{

    class CloneCommand: public UndoCommand
    {
    public:
        CloneCommand(UndoCommand *parent = nullptr);

        void undo();
        void redo();

        QList<quint64> itemIdList;
        QPointF translation;

    private:
        QList<quint64> cloneIdList;

    };

}
