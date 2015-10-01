#include "tool/graphicslinetool.h"
#include "item/graphicslineitem.h"

#include "handle/abstractgraphicshandle.h"
#include "schscene.h"
#include "schview.h"
#include "palette.h"

#include <QMouseEvent>
#include <QAction>

#include <QDebug>

GraphicsLineTool::GraphicsLineTool(QObject *parent):
    AbstractGraphicsInsertTool(parent), m_item(nullptr)
{
    QAction *action = new QAction(QIcon(":/icons/tool/graphicslinetool.svg"),
                           "Place a line", nullptr);
    action->setShortcut(QKeySequence("i,l"));
    setAction(action);
    setToolGroup("interactive-tools");
    setOperationWidget(nullptr);
    setOptionWidget(nullptr);
}

void GraphicsLineTool::cancel()
{
}

void GraphicsLineTool::activate(const QAction *which)
{
    Q_UNUSED(which);
}

void GraphicsLineTool::desactivate(const QAction *which)
{
    Q_UNUSED(which);
}

SchItem *GraphicsLineTool::beginInsert(const QPointF &pos)
{
    m_item = new GraphicsLineItem();
    QPen pen(SchView().palette()->violet(), 1);
    pen.setJoinStyle(Qt::RoundJoin);
    m_item->setPen(pen);
    m_item->setPos(pos);
    return m_item;
}

void GraphicsLineTool::addPoint(int idx, const QPointF &pos)
{
    Q_ASSERT(idx < 2);

    QPointF itemPos = m_item->mapFromScene(pos);
    if (idx == 0)
        setP1(itemPos);
    else {
        setP2(itemPos);
    }
}

void GraphicsLineTool::freezePoint(int idx, const QPointF &pos)
{
    Q_UNUSED(pos);
    if (idx == 0)
        return;

    emit objectInserted(m_item);
    resetTool();
}

bool GraphicsLineTool::removePoint(int idx, const QPointF &pos)
{
    Q_UNUSED(pos);
    Q_UNUSED(idx);
    return false; // Remove and delete line
}

void GraphicsLineTool::movePoint(int idx, const QPointF &pos)
{
    Q_ASSERT(idx < 2);

    QPointF itemPos = m_item->mapFromScene(pos);
    if (idx == 0)
        setP1(itemPos);
    else {
        setP2(itemPos);
    }
}

void GraphicsLineTool::endInsert(const QPointF &pos)
{
    Q_UNUSED(pos);
}

void GraphicsLineTool::cancelInsert()
{

}

void GraphicsLineTool::setP1(const QPointF &pos)
{
    QLineF line = m_item->line();
    line.setP1(pos);
    m_item->setLine(line);
}

void GraphicsLineTool::setP2(const QPointF &pos)
{
    QLineF line = m_item->line();
    line.setP2(pos);
    m_item->setLine(line);
}
