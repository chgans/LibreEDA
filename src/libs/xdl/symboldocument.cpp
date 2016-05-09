#include "xdl/symboldocument.h"

// TODO:
//  - import xsd stuff from leda-playground
//    See also: https://github.com/upverter/schematic-file-converter/blob/master/upconvert/parser/eaglexml/eagle.xsd
//  - Add tree model for document outline (INavigationView)
//  - Add X/Y mirror, locked and visible properties
//  - Add LabelData
//  - Add PinData (Do not use length, like eagle: point, short, middle and long

// Eagle symbol: polygon, wire, text, pin, circle, rectangle, frame
// gEDA symbol: line, picture, box, circle, arc, text, pin
// Kicad symbol: arc, circle, polygon, rectangle, text, bezier, pin




SymbolDocument::SymbolDocument():
  m_itemIndex(0)
{

}

QList<uint64_t> SymbolDocument::items() const
{
    return m_items.keys();
}

ItemData *SymbolDocument::item(uint64_t id) const
{
    if (!m_items.contains(id))
        return nullptr;
    return m_items.value(id);
}

QPointF SymbolDocument::origin() const
{
    return m_origin;
}

uint64_t SymbolDocument::addItem(ItemData *data)
{
    m_itemIndex++;
    data->itemId = m_itemIndex;
    m_items.insert(data->itemId, data);
    return data->itemId;
}

void SymbolDocument::removeItem(uint64_t id)
{
    if (!m_items.contains(id))
        return;
    ItemData *item = m_items.value(id);
    m_items.remove(id);
    delete item;
}

void SymbolDocument::setOrigin(const QPointF &origin)
{
    if (m_origin == origin)
        return;
    m_origin = origin;
}
