#ifndef GRAPHICSREGULARHANDLE_H
#define GRAPHICSREGULARHANDLE_H

#include "handle/abstractgraphicshandle.h"
#include <QGraphicsPathItem>

class RegularHandle : public Handle
{
public:
    explicit RegularHandle(Item *parent = nullptr);
    ~RegularHandle();

};

#endif // GRAPHICSREGULARHANDLE_H
