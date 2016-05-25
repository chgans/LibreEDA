#ifndef GRAPHICSITEMALIGNTOOL_H
#define GRAPHICSITEMALIGNTOOL_H

#include "tool/tool.h"
#include <QtGlobal>

namespace SymbolEditor
{

    class GraphicsItemAlignTool : public Tool
    {
        Q_OBJECT

    public:
        explicit GraphicsItemAlignTool(QObject *parent = nullptr);
        ~GraphicsItemAlignTool();

    public slots:
        void align(Qt::AlignmentFlag alignement);

    };

}

#endif // GRAPHICSITEMALIGNTOOL_H
