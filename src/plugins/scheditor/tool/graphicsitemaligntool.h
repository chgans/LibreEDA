#ifndef GRAPHICSITEMALIGNTOOL_H
#define GRAPHICSITEMALIGNTOOL_H

#include "tool/schtool.h"
#include <QtGlobal>

class GraphicsItemAlignTool : public SchTool
{
    Q_OBJECT

public:
    explicit GraphicsItemAlignTool(QObject *parent = nullptr);
    ~GraphicsItemAlignTool();

public slots:
    void align(Qt::AlignmentFlag alignement);

};

#endif // GRAPHICSITEMALIGNTOOL_H
