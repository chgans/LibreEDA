#include "tool/graphicslinetool.h"
#include "tool/graphicslinetooldialog.h"
#include "item/graphicslineitem.h"

#include "graphicshandle.h"
#include "graphicsscene.h"
#include "graphicsview.h"

#include <QMouseEvent>
#include <QAction>

#include <QDebug>

GraphicsLineTool::GraphicsLineTool(QObject *parent):
    AbstractGraphicsInsertTool(parent), m_item(nullptr)
{
    m_action = new QAction(QIcon(":/icons/tool/graphicslinetool.svg"),
                           "Place a line", nullptr);
    m_toolGroup = "interactive-tools";
}


QDialog *GraphicsLineTool::optionDialog()
{
    return new GraphicsLineToolDialog();
}

QString GraphicsLineTool::toolGroup() const
{
    return m_toolGroup;
}

QAction *GraphicsLineTool::action() const
{
    return m_action;
}

void GraphicsLineTool::cancel()
{
}

void GraphicsLineTool::activate(const QAction *which)
{
    Q_ASSERT(m_action == which);
}

void GraphicsLineTool::desactivate(const QAction *which)
{
    Q_ASSERT(m_action == which);
}

GraphicsObject *GraphicsLineTool::beginInsert(const QPointF &pos)
{
    m_item = new GraphicsLineItem();
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

void GraphicsLineTool::freezePoint(int idx)
{
    if (idx == 0)
        return;

    emit objectInserted(m_item);
    resetTool();
}

bool GraphicsLineTool::removePoint(int idx)
{
    Q_UNUSED(idx);
    return false;
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
