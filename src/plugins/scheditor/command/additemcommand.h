#ifndef ADDITEMCOMMAND_H
#define ADDITEMCOMMAND_H

#include <QUndoCommand>
#include <QUuid>
#include <QVariant>

class SchItem;
class SchDocument;

class Command: public QUndoCommand
{
public:
    Command(Command *parent = nullptr);

    SchDocument *document() const;
    void setDocument(SchDocument *doc);

private:
    SchDocument *m_document;
};

class AddItemCommand: public Command
{
public:
    AddItemCommand(const QString &type, const QUuid &uid, Command *parent = nullptr);

    void redo();
    void undo();

    QString itemType() const;
    QUuid itemId() const;

private:
    QString m_itemType;
    QUuid m_itemUid;
};

class SetItemPropertyCommand: public Command
{
public:
    SetItemPropertyCommand(const QUuid &uid, const QString &name, const QVariant &value, Command *parent = nullptr);

    void redo();
    void undo();

    QString propertyName() const;
    QVariant propertyValue() const;
    QUuid itemId() const;

private:
    QUuid m_itemUid;
    QString m_propertyName;
    QVariant m_propertyValue;
};

class RemoveItemCommand: public Command
{
public:
    RemoveItemCommand(const QUuid &uid, Command *parent = nullptr);

    void redo();
    void undo();
    QUuid itemId() const;

private:
    QUuid m_itemUid;
};
#endif // ADDITEMCOMMAND_H
