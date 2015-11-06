#include "tool/graphicsrecttool.h"
#include "item/graphicsrectitem.h"

#include "handle/abstractgraphicshandle.h"
#include "schscene.h"
#include "schview.h"
#include "palette.h"

#include <QMouseEvent>
#include <QKeyEvent>
#include <QAction>
#include <QDebug>

GraphicsRectTool::GraphicsRectTool(QObject *parent):
    AbstractGraphicsInsertTool(parent), m_item(nullptr)
{
    QAction *action = new QAction(QIcon(":/icons/tool/graphicsrecttool.svg"),
                           "Place a rectangle", nullptr);
    action->setShortcut(QKeySequence("i,r"));
    setAction(action);
    setToolGroup("interactive-tools");
    setOperationWidget(nullptr);
    setOptionWidget(nullptr);
}

GraphicsRectTool::~GraphicsRectTool()
{

}

void GraphicsRectTool::cancel()
{
}

SchItem *GraphicsRectTool::beginInsert(const QPointF &pos)
{
    m_item = new GraphicsRectItem();
    QPen pen(SchView().palette()->orange(), 1);
    pen.setJoinStyle(Qt::RoundJoin);
    m_item->setPen(pen);
    m_item->setBrush(QBrush(SchView().palette()->yellow()));
    m_item->setPos(pos);
    return m_item;
}

void GraphicsRectTool::addPoint(int idx, const QPointF &pos)
{
    Q_ASSERT(idx < 2);

    QPointF itemPos = m_item->mapFromScene(pos);
    if (idx == 0)
        setP1(itemPos);
    else {
        setP2(itemPos);
    }
}

void GraphicsRectTool::freezePoint(int idx, const QPointF &pos)
{
    Q_UNUSED(pos);
    Q_ASSERT(idx < 2);

    if (idx == 0)
        return;

    emit objectInserted(m_item);
    resetTool();
}

bool GraphicsRectTool::removePoint(int idx, const QPointF &pos)
{
    Q_UNUSED(pos);
    Q_UNUSED(idx);
    return false; // will remove and delete rect
}

void GraphicsRectTool::movePoint(int idx, const QPointF &pos)
{
    Q_ASSERT(idx < 2);

    QPointF itemPos = m_item->mapFromScene(pos);
    if (idx == 0)
        setP1(itemPos);
    else {
        setP2(itemPos);
    }
}

void GraphicsRectTool::endInsert(const QPointF &pos)
{
    Q_UNUSED(pos);
}

void GraphicsRectTool::cancelInsert()
{

}

void GraphicsRectTool::setP1(const QPointF &pos)
{
    m_rect.setTopLeft(pos);
    m_item->setRect(m_rect);
}

void GraphicsRectTool::setP2(const QPointF &pos)
{
    m_rect.setBottomRight(pos);
    m_item->setRect(m_rect);
}


void GraphicsRectTool::activate(const QAction *which)
{
    Q_UNUSED(which);
}

void GraphicsRectTool::desactivate(const QAction *which)
{
    Q_UNUSED(which);
}
