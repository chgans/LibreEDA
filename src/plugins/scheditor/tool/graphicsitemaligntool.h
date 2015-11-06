#ifndef GRAPHICSITEMALIGNTOOL_H
#define GRAPHICSITEMALIGNTOOL_H

#include "tool/schtool.h"
#include <QtGlobal>

class GraphicsItemAlignTool : public SchTool
{
public:
    GraphicsItemAlignTool();
    ~GraphicsItemAlignTool();

public slots:
    void align(Qt::AlignmentFlag alignement);

};

#endif // GRAPHICSITEMALIGNTOOL_H
