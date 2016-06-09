#include "document.h"

#include "xdl/symbol.h"
#include "xdl/symbolwriter.h"
#include "xdl/symbolreader.h"

#include <QFile>
#include <QUndoStack>
#include <QLoggingCategory>


// TODO: within namespace?
Q_DECLARE_LOGGING_CATEGORY(LedaSymbolDocumentLog)
Q_LOGGING_CATEGORY(LedaSymbolDocumentLog, "leda.sch.document")

using namespace SymbolEditor;

Document::Document(QObject *parent) :
    IDocument(parent),
    m_itemIndex(0)
{
    setModified(true);
}

bool Document::load(QString *errorString, const QString &fileName)
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
    for (auto item : symbol->drawingItems)
    {
        addItem(item);
    }

    return true;
}

const xdl::symbol::Item *Document::item(quint64 id) const
{
    if (!m_drawingItemMap.contains(id))
    {
        return nullptr;
    }
    return m_drawingItemMap.value(id);
}

QList<quint64> Document::itemIdList() const
{
    return m_drawingItemMap.keys();
}

quint64 Document::addItem(Document::Item *item)
{
    m_itemIndex++;
    m_drawingItemMap.insert(m_itemIndex, item);
    emit itemAdded(m_itemIndex, item);
    setModified(true);
    return m_itemIndex;
}

void Document::removeItem(quint64 id)
{
    if (!m_drawingItemMap.contains(id))
    {
        return;
    }

    auto item = m_drawingItemMap.value(id);
    m_drawingItemMap.remove(id);
    emit itemRemoved(id);
    setModified(true);
    delete item;
}

// FIXME: setModified, then emit? or emit aboutToChange()? Think Qt model/view framework
// FIXME: use if (itemOrWarning(itemId) == nullptr) { return; }
// FIXME: use if (!hasPropertyOrWarning(itemId, propertyId)) { return; }
// FIXME: Check QVariant can be converted?
// FIXME: quint64 vs int
void Document::setItemProperty(quint64 itemId, quint64 propertyId, const QVariant &value)
{
    if (!m_drawingItemMap.contains(itemId))
    {
        return;
    }
    auto item = m_drawingItemMap.value(itemId);

    item->setProperty(propertyId, value);
    emit itemChanged(itemId, item);
    emit itemPropertyChanged(itemId, propertyId, value);
    setModified(true);
    return;
}

QVariant Document::itemProperty(quint64 itemId, quint64 propertyId) const
{
    if (!m_drawingItemMap.contains(itemId))
    {
        return QVariant();
    }
    auto item = m_drawingItemMap.value(itemId);
    return item->property(propertyId);
}

QString Document::friendlyItemPropertyName(quint64 itemId, quint64 propertyId)
{
    if (!m_drawingItemMap.contains(itemId))
    {
        return "Unkown item";
    }
    auto item = m_drawingItemMap.value(itemId);
    return item->friendlyPropertyName(propertyId);
}

bool Document::save(QString *errorString, const QString &fileName)
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

void Document::render(QPainter *painter)
{
    Q_UNUSED(painter);
}
