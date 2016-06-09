#include "handle/regularhandle.h"

namespace SymbolEditor
{

    RegularHandle::RegularHandle(Item *parent):
        Handle(parent)
    {
        setFlag(QGraphicsItem::ItemIgnoresTransformations);
    }

    RegularHandle::~RegularHandle()
    {

    }

}
