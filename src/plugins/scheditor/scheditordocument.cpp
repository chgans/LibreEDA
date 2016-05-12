#include "scheditordocument.h"

#include "xdl/symbol.h"
#include "xdl/symbolwriter.h"
#include "xdl/symbolreader.h"

#include <QFile>
#include <QUndoStack>
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(Log)

Q_LOGGING_CATEGORY(Log, "leda.sch.document")

SchEditorDocument::SchEditorDocument(QObject *parent) :
    IDocument(parent),
    m_commandStack(new QUndoStack(this))
{
    setModified(true);
}

bool SchEditorDocument::load(QString *errorString, const QString &fileName)
{
    xdl::symbol::Reader reader;
    m_xdlSymbol = reader.read(fileName);

    if (m_xdlSymbol == nullptr)
    {
        *errorString = reader.errorString();
        return false;
    }

    return true;
}

void SchEditorDocument::executeCommand(SchCommand *command)
{
    Q_UNUSED(command);
    //m_commandStack->push(command);
}

QUndoStack *SchEditorDocument::undoStack()
{
    return m_commandStack;
}


bool SchEditorDocument::save(QString *errorString, const QString &fileName)
{
    xdl::symbol::Writer writer;
    if (!writer.write(fileName, m_xdlSymbol))
    {
        *errorString = writer.errorString();
        return false;
    }
    return true;
}

void SchEditorDocument::render(QPainter *painter)
{
    Q_UNUSED(painter);
}
