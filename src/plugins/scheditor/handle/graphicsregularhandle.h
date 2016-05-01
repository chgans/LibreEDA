#ifndef GRAPHICSREGULARHANDLE_H
#define GRAPHICSREGULARHANDLE_H

#include "handle/abstractgraphicshandle.h"
#include <QGraphicsPathItem>

class GraphicsRegularHandle : public AbstractGraphicsHandle
{
public:
    explicit GraphicsRegularHandle(SchItem *parent = nullptr);
    ~GraphicsRegularHandle();

};

#endif // GRAPHICSREGULARHANDLE_H
