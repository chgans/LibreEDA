#include "handle/regularhandle.h"

RegularHandle::RegularHandle(Item *parent):
    Handle(parent)
{
    setFlag(QGraphicsItem::ItemIgnoresTransformations);
}

RegularHandle::~RegularHandle()
{

}


