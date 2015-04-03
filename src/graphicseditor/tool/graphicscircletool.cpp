#include "graphicscircletool.h"

#include "item/graphicscircleitem.h"

#include <QAction>

GraphicsCircleTool::GraphicsCircleTool(QObject *parent):
    AbstractGraphicsInsertTool(parent)
{
    m_toolAction = new QAction(QIcon(":/icons/tool/graphicscircletool.svg"),
                           "Place a circle", nullptr);
    m_toolAction->setShortcut(QKeySequence("i,c"));
    m_toolGroup = "interactive-tools";
}

GraphicsCircleTool::~GraphicsCircleTool()
{
}

QWidget *GraphicsCircleTool::taskWidget()
{
    return nullptr;
}

QWidget *GraphicsCircleTool::optionWidget()
{
    return nullptr;
}

QString GraphicsCircleTool::toolGroup() const
{
    return m_toolGroup;
}

QAction *GraphicsCircleTool::action() const
{
    return m_toolAction;
}

void GraphicsCircleTool::activate(const QAction *which)
{
    Q_ASSERT(m_toolAction == which);
}

void GraphicsCircleTool::desactivate(const QAction *which)
{
    Q_ASSERT(m_toolAction == which);
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
    Q_ASSERT(idx < 2);

    if ( idx == 0)
        return;

    emit objectInserted(m_item);
    resetTool();
}

bool GraphicsCircleTool::removePoint(int idx, const QPointF &pos)
{
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
