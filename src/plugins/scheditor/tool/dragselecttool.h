#ifndef DRAGSELECTTOOL_H
#define DRAGSELECTTOOL_H

#include "scheditor/tool/interactivetool.h"

#include <QPointF>

class QRubberBand;

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
};

#endif // DRAGSELECTTOOL_H
