#include "document/schdocument.h"

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
SchDocument::SchDocument(QObject *parent):
    QObject(parent)
{

}

const SchDocumentObject *SchDocument::documentObject(const QUuid &uid) const
{
    return nullptr;
}
