#include "command/additemcommand.h"


Command::Command(Command *parent):
    QUndoCommand(parent)
{

}

SchDocument *Command::document() const
{
    return m_document;
}

void Command::setDocument(SchDocument *doc)
{
    m_document = doc;
}

AddItemCommand::AddItemCommand(const QString &type, const QUuid &uid, Command *parent):
    Command(parent),
    m_itemType(type),
    m_itemUid(uid)
{
    setText("Add item");
}

void AddItemCommand::redo()
{

}

void AddItemCommand::undo()
{

}

QString AddItemCommand::itemType() const
{
    return m_itemType;
}

QUuid AddItemCommand::itemId() const
{
    return m_itemUid;
}

/*
 * flags
 * opacity
 * parentItem
 * rotation
 * scale
 * scenePos
 * zValue
 *
 */
SetItemPropertyCommand::SetItemPropertyCommand(const QUuid &uid, const QString &name,
                                               const QVariant &value, Command *parent):
    Command(parent),
    m_itemUid(uid),
    m_propertyName(name),
    m_propertyValue(value)
{
    setText("Modify item");
}

void SetItemPropertyCommand::redo()
{

}

void SetItemPropertyCommand::undo()
{

}

QString SetItemPropertyCommand::propertyName() const
{
    return m_propertyName;
}

QVariant SetItemPropertyCommand::propertyValue() const
{
    return m_propertyValue;
}

QUuid SetItemPropertyCommand::itemId() const
{
    return m_itemUid;
}

RemoveItemCommand::RemoveItemCommand(const QUuid &uid, Command *parent):
    Command(parent),
    m_itemUid(uid)
{
    setText("Remove item");
}

void RemoveItemCommand::redo()
{

}

void RemoveItemCommand::undo()
{

}

QUuid RemoveItemCommand::itemId() const
{
    return m_itemUid;
}
