#pragma once

#include "command.h"

namespace SymbolEditor
{

    class SetPropertyCommand : public UndoCommand
    {
    public:
        SetPropertyCommand(UndoCommand *parent = nullptr);

        void setItemId(quint64 id);
        void setPropertId(quint64 id);
        void setPropertyValue(const QVariant &value);

    private:
        quint64 m_itemId;
        quint64 m_propertyId;
        QVariant m_originalValue;
        QVariant m_newValue;

        // QUndoCommand interface
    public:
        void undo() override;
        void redo() override;
    };

}
