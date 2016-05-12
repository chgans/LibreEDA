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
    auto symbol = reader.read(fileName); // FIXME: leak/ownership
    if (symbol == nullptr)
    {
        *errorString = reader.errorString();
        return false;
    }

    m_symbolName = symbol->name;
    m_symbolLabel = symbol->description;
    quint64 idx = 0;
    for (auto item: symbol->drawingItems)
    {
        m_drawingItemMap.insert(idx++, item);
    }

    auto circle = new xdl::symbol::CircleItem;
    circle->radius = 50;
    m_drawingItemMap.insert(idx++, circle);
    auto ellipse = new xdl::symbol::EllipseItem;
    ellipse->xRadius = 50;
    ellipse->yRadius = 100;
    ellipse->position = QPointF(100, 100);
    m_drawingItemMap.insert(idx++, ellipse);

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

const xdl::symbol::Item *SchEditorDocument::drawingItem(quint64 id) const
{
    if (!m_drawingItemMap.contains(id))
    {
        return nullptr;
    }
    return m_drawingItemMap.value(id);
}

QList<quint64> SchEditorDocument::drawingItemIdList() const
{
    return m_drawingItemMap.keys();
}

bool SchEditorDocument::save(QString *errorString, const QString &fileName)
{
    xdl::symbol::Writer writer;
    xdl::symbol::Symbol symbol;
    symbol.name = m_symbolName;
    symbol.description = m_symbolLabel;
    symbol.drawingItems = m_drawingItemMap.values();
    if (!writer.write(fileName, &symbol))
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
