#pragma once

#include "command.h"

namespace SymbolEditor
{

    class TranslateCommand: public UndoCommand
    {
    public:
        TranslateCommand(UndoCommand *parent = nullptr);

        void undo();
        void redo();

        QList<quint64> itemIdList;
        QPointF amount;
    };

}
