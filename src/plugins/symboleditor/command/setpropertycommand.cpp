#include "symboleditor/command/setpropertycommand.h"
#include "symboleditor/document.h"

namespace SymbolEditor
{

    SetPropertyCommand::SetPropertyCommand(UndoCommand *parent):
        UndoCommand(parent)
    {
    }

    void SetPropertyCommand::setItemId(quint64 id)
    {
        m_itemId = id;
    }

    void SetPropertyCommand::setPropertId(quint64 id)
    {
        m_propertyId = id;
    }

    void SetPropertyCommand::setPropertyValue(const QVariant &value)
    {
        m_newValue = value;
    }

    void SetPropertyCommand::undo()
    {
        document()->setItemProperty(m_itemId, m_propertyId, m_originalValue);
    }

    void SetPropertyCommand::redo()
    {
        m_originalValue = document()->itemProperty(m_itemId, m_propertyId);
        document()->setItemProperty(m_itemId, m_propertyId, m_newValue);
        setText(QString("Set '%1' property").arg(document()->friendlyItemPropertyName(m_itemId, m_propertyId)));
    }

}


