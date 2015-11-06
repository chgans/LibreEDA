#include "handle/graphicsregularhandle.h"

GraphicsRegularHandle::GraphicsRegularHandle(SchItem *parent):
    AbstractGraphicsHandle(parent)
{
    setFlag(QGraphicsItem::ItemIgnoresTransformations);
}

GraphicsRegularHandle::~GraphicsRegularHandle()
{

}


