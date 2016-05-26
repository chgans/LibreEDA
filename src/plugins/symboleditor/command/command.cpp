#include "command.h"
#include "document.h"

#include <QLoggingCategory>

// TODO: Private NS?
Q_DECLARE_LOGGING_CATEGORY(LedaSymbolCommand)
Q_LOGGING_CATEGORY(LedaSymbolCommand, "leda.sch.command")

using namespace SymbolEditor;

UndoCommand::UndoCommand(UndoCommand *parent):
    QUndoCommand(parent), m_document(nullptr)
{

}

void UndoCommand::setDocument(Document *document)
{
    m_document = document;
}

Document *UndoCommand::document() const
{
    return m_document;
}

void UndoCommand::warnItemNotFound(const QString command, quint64 id)
{
    qCWarning(LedaSymbolCommand) << QString("%1: Item '%2' not found").arg(command).arg(id);
}
