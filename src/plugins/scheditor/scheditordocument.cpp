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
    m_itemIndex(0)
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
    m_itemIndex = 0;
    m_drawingItemMap.clear();
    for (auto item: symbol->drawingItems)
    {
        addDrawingItem(item);
    }

    return true;
}

const xdl::symbol::Item *SchEditorDocument::drawingItem(quint64 id) const
{
    if (!m_drawingItemMap.contains(id))
    {
        return nullptr;
    }
    return m_drawingItemMap.value(id);
}

SchEditorDocument::Item *SchEditorDocument::drawingItem(quint64 id)
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

quint64 SchEditorDocument::addDrawingItem(SchEditorDocument::Item *item)
{
    m_itemIndex++;
    m_drawingItemMap.insert(m_itemIndex, item);
    emit drawingItemAdded(m_itemIndex, item);
    setModified(true);
    return m_itemIndex;
}

void SchEditorDocument::replaceDrawingItem(quint64 id, SchEditorDocument::Item *item)
{
    if (!m_drawingItemMap.contains(id))
    {
        delete item;
        return;
    }

    auto oldItem = m_drawingItemMap.value(id);
    m_drawingItemMap.insert(id, item);
    emit drawingItemChanged(id, item);
    setModified(true);
    delete oldItem;
}

void SchEditorDocument::removeDrawingItem(quint64 id)
{
    if (!m_drawingItemMap.contains(id))
    {
        return;
    }

    auto item = m_drawingItemMap.value(id);
    m_drawingItemMap.remove(id);
    emit drawingItemRemoved(id);
    setModified(true);
    delete item;
}

void SchEditorDocument::updateDrawingItem(quint64 id)
{
    if (!m_drawingItemMap.contains(id))
    {
        return;
    }

    auto item = m_drawingItemMap.value(id);
    emit drawingItemChanged(id, item);
    setModified(true);
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
    setModified(false);
    return true;
}

void SchEditorDocument::render(QPainter *painter)
{
    Q_UNUSED(painter);
}
