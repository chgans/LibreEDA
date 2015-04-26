#include "graphicscircletool.h"

#include "item/graphicscircleitem.h"

#include <QAction>

GraphicsCircleTool::GraphicsCircleTool(QObject *parent):
    AbstractGraphicsInsertTool(parent)
{
    QAction *action = new QAction(QIcon(":/icons/tool/graphicscircletool.svg"),
                                  "Place a circle", nullptr);
    action->setShortcut(QKeySequence("i,c"));
    setAction(action);
    setToolGroup("interactive-tools");
    setOperationWidget(nullptr);
    setOptionWidget(nullptr);
}

GraphicsCircleTool::~GraphicsCircleTool()
{
}

void GraphicsCircleTool::activate(const QAction *which)
{
    Q_UNUSED(which);
}

void GraphicsCircleTool::desactivate(const QAction *which)
{
    Q_UNUSED(which);
}

GraphicsObject *GraphicsCircleTool::beginInsert(const QPointF &pos)
{
    m_item = new GraphicsCircleItem();
    m_item->setPos(pos);
    return m_item;
}

void GraphicsCircleTool::addPoint(int idx, const QPointF &pos)
{
    Q_ASSERT(idx < 2);

    if ( idx == 0)
        return;

    QPointF itemPos = m_item->mapFromScene(pos);
    m_item->setRadius(qAbs(itemPos.x()));
}

void GraphicsCircleTool::freezePoint(int idx, const QPointF &pos)
{
    Q_UNUSED(pos);
    Q_ASSERT(idx < 2);

    if ( idx == 0)
        return;

    emit objectInserted(m_item);
    resetTool();
}

bool GraphicsCircleTool::removePoint(int idx, const QPointF &pos)
{
    Q_UNUSED(pos);
    Q_UNUSED(idx);
    return false; // Remove and delete circles
}

void GraphicsCircleTool::movePoint(int idx, const QPointF &pos)
{
    Q_ASSERT(idx == 1);

     QPointF itemPos = m_item->mapFromScene(pos);
     m_item->setRadius(qAbs(itemPos.x()));
}

void GraphicsCircleTool::endInsert(const QPointF &pos)
{
    Q_UNUSED(pos);
}

void GraphicsCircleTool::cancelInsert()
{
}
