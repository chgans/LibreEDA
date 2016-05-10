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


namespace xdl { namespace symbol {

Document::Document():
  m_itemIndex(0)
{

}

QList<uint64_t> Document::items() const
{
    return m_items.keys();
}

Item *Document::item(uint64_t id) const
{
    if (!m_items.contains(id))
        return nullptr;
    return m_items.value(id);
}

QPointF Document::origin() const
{
    return m_origin;
}

QString Document::symbolName() const
{
    return "fixme: symbol name";
}

QString Document::symbolDescription() const
{
    return "fixme: symbol desc";
}

uint64_t Document::addItem(Item *item)
{
    m_itemIndex++;
    item->itemId = m_itemIndex;
    m_items.insert(item->itemId, item);
    return item->itemId;
}

void Document::removeItem(uint64_t id)
{
    if (!m_items.contains(id))
        return;
    Item *item = m_items.value(id);
    m_items.remove(id);
    delete item;
}

void Document::setOrigin(const QPointF &origin)
{
    if (m_origin == origin)
        return;
    m_origin = origin;
}

}}
