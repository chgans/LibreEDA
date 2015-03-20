#ifndef GRAPHICSITEMALIGNTOOL_H
#define GRAPHICSITEMALIGNTOOL_H

#include "graphicstool.h"
#include <QtGlobal>

class GraphicsItemAlignTool : public GraphicsTool
{
public:
    GraphicsItemAlignTool();
    ~GraphicsItemAlignTool();

public slots:
    void align(Qt::AlignmentFlag alignement);

};

#endif // GRAPHICSITEMALIGNTOOL_H
