#include "tool/graphicsbeziertool.h"
#include "item/graphicsbezieritem.h"

#include "graphicshandle.h"
#include "graphicsview.h"
#include "graphicsscene.h"

#include <QMouseEvent>

#include <QAction>
#include <QIcon>

#include <QLoggingCategory>
Q_DECLARE_LOGGING_CATEGORY(GraphicsBezierToolLog)
Q_LOGGING_CATEGORY(GraphicsBezierToolLog, "graphics.bezier.tool")
#define DEBUG() qCDebug(GraphicsBezierToolLog)
#define WARNING() qCDebug(GraphicsBezierToolLog)

#define WARN_STATE WARNING() << "Inconsistent state!"


GraphicsBezierTool::GraphicsBezierTool(QObject *parent):
    AbstractGraphicsInsertTool(parent),
    m_item(nullptr)
{
    m_toolAction = new QAction(QIcon(":/icons/tool/graphicsbeziertool.svg"),
                           "Place a bezier curve", nullptr);
    m_toolAction->setShortcut(QKeySequence("i,b"));
    m_toolGroup = "interactive-tools";
}

GraphicsBezierTool::~GraphicsBezierTool()
{

}

QDialog *GraphicsBezierTool::optionDialog()
{
    return nullptr;
}

QString GraphicsBezierTool::toolGroup() const
{
    return m_toolGroup;
}

QAction *GraphicsBezierTool::action() const
{
    return m_toolAction;
}

void GraphicsBezierTool::activate(const QAction *which)
{
    Q_ASSERT(m_toolAction == which);
}

void GraphicsBezierTool::desactivate(const QAction *which)
{
    Q_ASSERT(m_toolAction == which);
}

GraphicsObject *GraphicsBezierTool::beginInsert(const QPointF &pos)
{
    m_item = new GraphicsBezierItem();
    m_item->setPos(pos);
    return m_item;
}

void GraphicsBezierTool::addPoint(int idx, const QPointF &pos)
{
    Q_UNUSED(idx);
    m_item->addPoint(m_item->mapFromScene(pos));
}

void GraphicsBezierTool::freezePoint(int idx, const QPointF &pos)
{
    Q_UNUSED(idx);
}

bool GraphicsBezierTool::removePoint(int idx, const QPointF &pos)
{
    QPointF oldPos = m_item->pointAt(idx);
    m_item->removePoint(idx);
    if (m_item->pointCount() > 1) {
        m_item->movePoint(idx-1, oldPos);
        return true; // Keep going
    }
    else {
        return false; // remove and delete bezier
    }
    return false;
}

void GraphicsBezierTool::movePoint(int idx, const QPointF &pos)
{
    m_item->movePoint(idx, m_item->mapFromScene(pos));
}

void GraphicsBezierTool::endInsert(const QPointF &pos)
{
    Q_UNUSED(pos);
    emit objectInserted(m_item);
    m_item = nullptr;
    resetTool();
}

void GraphicsBezierTool::cancelInsert()
{

}

