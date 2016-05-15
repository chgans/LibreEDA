#ifndef MOVEITEMTOOL_H
#define MOVEITEMTOOL_H

#include "scheditor/tool/interactivetool.h"

#include <QPointF>

class SchItem;

class MoveItemTool : public InteractiveTool
{
public:
    MoveItemTool(QObject *parent = nullptr);

private:
    QList<SchItem *> m_items;
    QPointF m_pressPosition;
    QPointF m_lastPosition;

    // InteractiveTool interface
public:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
};

#endif // MOVEITEMTOOL_H
