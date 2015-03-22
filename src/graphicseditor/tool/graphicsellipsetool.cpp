#include "graphicsellipsetool.h"
#include "item/graphicsellipseitem.h"

#include <QAction>

GraphicsEllipseTool::GraphicsEllipseTool(QObject *parent):
    AbstractGraphicsInsertTool(parent)
{
    m_action = new QAction(QIcon(":/icons/tool/graphicsellipsetool.svg"),
                           "Place an ellipse", nullptr);;
    m_toolGroup = "interactive-tools";
}

GraphicsEllipseTool::~GraphicsEllipseTool()
{

}

QDialog *GraphicsEllipseTool::optionDialog()
{
    return nullptr;
}

QString GraphicsEllipseTool::toolGroup() const
{
    return m_toolGroup;
}

QAction *GraphicsEllipseTool::action() const
{
    return m_action;
}

void GraphicsEllipseTool::activate(const QAction *which)
{
    Q_ASSERT(m_action == which);
}

void GraphicsEllipseTool::desactivate(const QAction *which)
{
    Q_ASSERT(m_action == which);
}

GraphicsObject *GraphicsEllipseTool::beginInsert(const QPointF &pos)
{
    m_item = new GraphicsEllipseItem();
    m_item->setPos(pos);
    return m_item;
}

void GraphicsEllipseTool::addPoint(int idx, const QPointF &pos)
{
    Q_ASSERT(idx < 3);

    if ( idx == 0)
        return;

    QPointF itemPos = m_item->mapFromScene(pos);

    if (idx == 1)
        m_item->setXRadius(qAbs(itemPos.x()));
    else
        m_item->setYRadius(qAbs(itemPos.y()));
}

void GraphicsEllipseTool::freezePoint(int idx)
{
    Q_ASSERT(idx < 3);

    if ( idx != 2)
        return;

    emit objectInserted(m_item);
    resetTool();
}

bool GraphicsEllipseTool::removePoint(int idx)
{
    return false;
}

void GraphicsEllipseTool::movePoint(int idx, const QPointF &pos)
{
    Q_ASSERT(idx < 3);

    if ( idx == 0)
        return;

    QPointF itemPos = m_item->mapFromScene(pos);

    if (idx == 1) {
        m_item->setXRadius(qAbs(itemPos.x()));
        m_item->setYRadius(qAbs(itemPos.x()));
    }
    else
        m_item->setYRadius(qAbs(itemPos.y()));
}

void GraphicsEllipseTool::endInsert(const QPointF &pos)
{
    Q_UNUSED(pos);
}

void GraphicsEllipseTool::cancelInsert()
{
}
