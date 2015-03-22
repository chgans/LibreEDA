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
    m_action = new QAction(QIcon(":/icons/graphicsbeziertool.svg"),
                           "Place a bezier curve", nullptr);;
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
    return m_action;
}

void GraphicsBezierTool::activate(const QAction *which)
{
    Q_ASSERT(m_action == which);
}

void GraphicsBezierTool::desactivate(const QAction *which)
{
    Q_ASSERT(m_action == which);
}

GraphicsObject *GraphicsBezierTool::beginInsert(const QPointF &pos)
{
    m_item = new GraphicsBezierItem();
    m_item->setPos(pos);
    m_item->setPen(QPen(QBrush(QColor::fromRgb(0x00, 0x00, 0x80)), 0.5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    m_item->setFlags(QGraphicsItem::ItemIsSelectable|QGraphicsItem::ItemIsMovable);
    return m_item;
}

void GraphicsBezierTool::addPoint(int idx, const QPointF &pos)
{
    Q_UNUSED(idx);
    m_item->addPoint(m_item->mapFromScene(pos));
}

void GraphicsBezierTool::freezePoint(int idx)
{
    Q_UNUSED(idx);
}

bool GraphicsBezierTool::removePoint(int idx)
{
    Q_UNUSED(idx);
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

