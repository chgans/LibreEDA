#pragma once

#include "tool/tool.h"
#include <QtGlobal>

namespace SymbolEditor
{

    class GraphicsItemAlignTool : public Tool
    {
        Q_OBJECT

    public:
        explicit GraphicsItemAlignTool(Editor *editor);
        ~GraphicsItemAlignTool();

    public slots:
        void align(Qt::AlignmentFlag alignement);

    };

}
