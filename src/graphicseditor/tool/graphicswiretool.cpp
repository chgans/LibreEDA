#include "tool/graphicswiretool.h"
#include "item/graphicswireitem.h"

#include "graphicshandle.h"
#include "graphicsscene.h"
#include "graphicsview.h"

#include <QMouseEvent>
#include <QKeyEvent>
#include <QAction>

GraphicsWireTool::GraphicsWireTool(QObject *parent):
    AbstractGraphicsInsertTool(parent)
{
    m_toolAction = new QAction(QIcon(":/icons/tool/graphicspolylinetool.svg"), // TODO: rename to wire
                               "Place a wire", nullptr);;
    m_toolGroup = "interactive-tools";
}

GraphicsWireTool::~GraphicsWireTool()
{
}

QDialog *GraphicsWireTool::optionDialog()
{
    return nullptr;
}

QString GraphicsWireTool::toolGroup() const
{
    return m_toolGroup;
}

QAction *GraphicsWireTool::action() const
{
    return m_toolAction;
}

void GraphicsWireTool::activate(const QAction *which)
{
    Q_ASSERT(m_toolAction == which);
}

void GraphicsWireTool::desactivate(const QAction *which)
{
    Q_ASSERT(m_toolAction == which);
}

GraphicsObject *GraphicsWireTool::beginInsert(const QPointF &pos)
{
    m_item = new GraphicsWireItem();
    m_item->setPos(pos);
    return m_item;
}

void GraphicsWireTool::addPoint(int idx, const QPointF &pos)
{
    Q_UNUSED(idx);
    m_item->addPoint(m_item->mapFromScene(pos));
}

void GraphicsWireTool::freezePoint(int idx)
{
    Q_UNUSED(idx);
}

bool GraphicsWireTool::removePoint(int idx)
{
    Q_UNUSED(idx);
    return false;
}

void GraphicsWireTool::movePoint(int idx, const QPointF &pos)
{
    m_item->movePoint(idx, m_item->mapFromScene(pos));
}

void GraphicsWireTool::endInsert(const QPointF &pos)
{
    Q_UNUSED(pos);
    emit objectInserted(m_item);
    m_item = nullptr;
    resetTool();
}

void GraphicsWireTool::cancelInsert()
{
}
