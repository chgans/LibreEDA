#include "handle/regularhandle.h"

using namespace SymbolEditor;

RegularHandle::RegularHandle(Item *parent):
    Handle(parent)
{
    setFlag(QGraphicsItem::ItemIgnoresTransformations);
}

RegularHandle::~RegularHandle()
{

}


