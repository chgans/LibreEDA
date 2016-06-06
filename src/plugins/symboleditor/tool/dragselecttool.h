#pragma once

#include "tool/interactivetool.h"

#include <QPointF>

class QRubberBand;

namespace SymbolEditor
{

    class DragSelectTool: public InteractiveTool
    {
    public:
        DragSelectTool(QObject *parent = nullptr);

    private:
        QPointF m_pressPosition;
        QPointF m_lastPosition;
        QRubberBand *m_rubberBand;

        // InteractiveTool interface
    public:
        void mousePressEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        void mouseReleaseEvent(QMouseEvent *event);

        // Tool interface
    public:
        void activate(View *view);
        void desactivate();
    };

}
