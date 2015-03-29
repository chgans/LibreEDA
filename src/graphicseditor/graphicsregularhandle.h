#ifndef GRAPHICSREGULARHANDLE_H
#define GRAPHICSREGULARHANDLE_H

#include "graphicseditor/abstractgraphicshandle.h"
#include <QGraphicsPathItem>

class GraphicsRegularHandle : public AbstractGraphicsHandle
{
public:
    GraphicsRegularHandle(GraphicsObject *parent = 0);
    ~GraphicsRegularHandle();

};

#endif // GRAPHICSREGULARHANDLE_H
