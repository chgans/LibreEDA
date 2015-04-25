#include "graphicsregularhandle.h"

GraphicsRegularHandle::GraphicsRegularHandle(GraphicsObject *parent):
    AbstractGraphicsHandle(parent)
{
    setFlag(QGraphicsItem::ItemIgnoresTransformations);
}

GraphicsRegularHandle::~GraphicsRegularHandle()
{

}


