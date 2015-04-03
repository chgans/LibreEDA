#include "graphicsellipsetool.h"
#include "item/graphicsellipseitem.h"

#include <QAction>

GraphicsEllipseTool::GraphicsEllipseTool(QObject *parent):
    AbstractGraphicsInsertTool(parent)
{
    m_toolAction = new QAction(QIcon(":/icons/tool/graphicsellipsetool.svg"),
                           "Place an ellipse", nullptr);
    m_toolAction->setShortcut(QKeySequence("i,e"));
    m_toolGroup = "interactive-tools";
}

GraphicsEllipseTool::~GraphicsEllipseTool()
{

}

QWidget *GraphicsEllipseTool::taskWidget()
{
    return nullptr;
}

QWidget *GraphicsEllipseTool::optionWidget()
{
    return nullptr;
}

QString GraphicsEllipseTool::toolGroup() const
{
    return m_toolGroup;
}

QAction *GraphicsEllipseTool::action() const
{
    return m_toolAction;
}

void GraphicsEllipseTool::activate(const QAction *which)
{
    Q_ASSERT(m_toolAction == which);
}

void GraphicsEllipseTool::desactivate(const QAction *which)
{
    Q_ASSERT(m_toolAction == which);
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

void GraphicsEllipseTool::freezePoint(int idx, const QPointF &pos)
{
    Q_ASSERT(idx < 3);

    if ( idx != 2)
        return;

    emit objectInserted(m_item);
    resetTool();
}

bool GraphicsEllipseTool::removePoint(int idx, const QPointF &pos)
{
    if (idx == 2) {
        QPointF itemPos = m_item->mapFromScene(pos);
        m_item->setYRadius(qAbs(itemPos.y()));
        return true; // Keep going
    }
    else
        return false; // Remove and delete ellipse
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
