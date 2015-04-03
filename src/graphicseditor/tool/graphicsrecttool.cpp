#include "tool/graphicsrecttool.h"
#include "item/graphicsrectitem.h"

#include "graphicseditor/abstractgraphicshandle.h"
#include "graphicsscene.h"
#include "graphicsview.h"

#include <QMouseEvent>
#include <QKeyEvent>
#include <QAction>

GraphicsRectTool::GraphicsRectTool(QObject *parent):
    AbstractGraphicsInsertTool(parent), m_item(nullptr)
{
    m_toolAction = new QAction(QIcon(":/icons/tool/graphicsrecttool.svg"),
                           "Place a rectangle", nullptr);
    m_toolAction->setShortcut(QKeySequence("i,r"));
    m_toolGroup = "interactive-tools";
}

GraphicsRectTool::~GraphicsRectTool()
{

}

QWidget *GraphicsRectTool::taskWidget()
{
    return nullptr;
}

QWidget *GraphicsRectTool::optionWidget()
{
    return nullptr;
}

QString GraphicsRectTool::toolGroup() const
{
    return m_toolGroup;
}

QAction *GraphicsRectTool::action() const
{
    return m_toolAction;
}

void GraphicsRectTool::cancel()
{
}

GraphicsObject *GraphicsRectTool::beginInsert(const QPointF &pos)
{
    m_item = new GraphicsRectItem();
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
    Q_ASSERT(idx < 2);

    if (idx == 0)
        return;

    emit objectInserted(m_item);
    resetTool();
}

bool GraphicsRectTool::removePoint(int idx, const QPointF &pos)
{
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
    QRectF rect = m_item->rect();
    rect.setTopLeft(pos);
    m_item->setRect(rect);
}

void GraphicsRectTool::setP2(const QPointF &pos)
{
    QRectF rect = m_item->rect();
    rect.setBottomLeft(pos);
    m_item->setRect(rect);
}


void GraphicsRectTool::activate(const QAction *which)
{
    Q_ASSERT(which == m_toolAction);
}

void GraphicsRectTool::desactivate(const QAction *which)
{
    Q_ASSERT(which == m_toolAction);
}
