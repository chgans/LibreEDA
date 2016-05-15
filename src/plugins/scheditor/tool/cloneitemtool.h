#ifndef CLONEITEMTOOL_H
#define CLONEITEMTOOL_H

#include "scheditor/tool/interactivetool.h"

#include <QPointF>

class SchItem;

class CloneItemTool : public InteractiveTool
{
public:
    CloneItemTool(QObject *parent = nullptr);

protected:
    SchItem *cloneItem(SchItem *item);
    QList<SchItem *> cloneItems(QList<SchItem *> items);

private:
    QList<SchItem *> m_items;
    QList<SchItem *> m_clonedItems;
    QPointF m_pressPosition;
    QPointF m_lastPosition;

    // InteractiveTool interface
public:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
};

#endif // CLONEITEMTOOL_H
