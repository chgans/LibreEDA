#ifndef GRAPHICSREGULARHANDLE_H
#define GRAPHICSREGULARHANDLE_H

#include "handle/handle.h"
#include <QGraphicsPathItem>

class RegularHandle : public Handle
{
public:
    explicit RegularHandle(Item *parent = nullptr);
    ~RegularHandle();

};

#endif // GRAPHICSREGULARHANDLE_H
