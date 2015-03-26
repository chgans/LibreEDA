#include "graphicspolygontool.h"
#include "item/graphicspolygonitem.h"

#include <QPolygonF>

#include <QAction>

GraphicsPolygonTool::GraphicsPolygonTool(QObject *parent):
    AbstractGraphicsInsertTool(parent)
{
    m_action = new QAction(QIcon(":/icons/tool/graphicspolygontool.svg"),
                           "Place a rectangle", nullptr);
    m_toolGroup = "interactive-tools";
}

GraphicsPolygonTool::~GraphicsPolygonTool()
{

}

QDialog *GraphicsPolygonTool::optionDialog()
{
    return nullptr;
}

QString GraphicsPolygonTool::toolGroup() const
{
    return m_toolGroup;
}

QAction *GraphicsPolygonTool::action() const
{
    return m_action;
}

void GraphicsPolygonTool::activate(const QAction *which)
{
    Q_ASSERT(m_action == which);
}

void GraphicsPolygonTool::desactivate(const QAction *which)
{
    Q_ASSERT(m_action == which);
}

GraphicsObject *GraphicsPolygonTool::beginInsert(const QPointF &pos)
{
    m_item = new GraphicsPolygonItem();
    m_item->setPos(pos);
    return m_item;
}

void GraphicsPolygonTool::addPoint(int idx, const QPointF &pos)
{
    QPointF itemPos = m_item->mapFromScene(pos);
    m_item->addPoint(itemPos);
}

void GraphicsPolygonTool::freezePoint(int idx)
{
    Q_UNUSED(idx);
}

bool GraphicsPolygonTool::removePoint(int idx)
{
    QPolygonF poly = m_item->polygon();
    QPointF point = poly[idx];
    poly.removeAt(idx);
    if (poly.count() > 1)
        poly[idx - 1] = point;
    m_item->setPolygon(poly);
    Q_UNUSED(idx);
    return false;
}

void GraphicsPolygonTool::movePoint(int idx, const QPointF &pos)
{
    QPointF itemPos = m_item->mapFromScene(pos);
    m_item->movePoint(idx, itemPos);
}

void GraphicsPolygonTool::endInsert(const QPointF &pos)
{
    Q_UNUSED(pos);
    emit objectInserted(m_item);
    m_item = nullptr;
    resetTool();
}

void GraphicsPolygonTool::cancelInsert()
{
}
