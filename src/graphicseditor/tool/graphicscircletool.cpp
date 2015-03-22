#include "graphicscircletool.h"

#include "graphicscircleitem.h"

#include <QAction>

GraphicsCircleTool::GraphicsCircleTool(QObject *parent):
    AbstractGraphicsInsertTool(parent)
{
    m_action = new QAction(QIcon(":/icons/graphicscircletool.svg"),
                           "Place a circle", nullptr);;
    m_toolGroup = "interactive-tools";
}

GraphicsCircleTool::~GraphicsCircleTool()
{
}

QDialog *GraphicsCircleTool::optionDialog()
{
    return nullptr;
}

QString GraphicsCircleTool::toolGroup() const
{
    return m_toolGroup;
}

QAction *GraphicsCircleTool::action() const
{
    return m_action;
}

void GraphicsCircleTool::activate(const QAction *which)
{
    Q_ASSERT(m_action == which);
}

void GraphicsCircleTool::desactivate(const QAction *which)
{
    Q_ASSERT(m_action == which);
}

GraphicsObject *GraphicsCircleTool::beginInsert(const QPointF &pos)
{
    m_item = new GraphicsCircleItem();
    m_item->setPos(pos);
    m_item->setPen(QPen(QBrush(QColor::fromRgb(0x80, 0x00, 0x00)), 0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    m_item->setBrush(QBrush(QColor::fromRgb(0xff, 0xff, 0xb0)));
    m_item->setFlags(QGraphicsItem::ItemIsSelectable|QGraphicsItem::ItemIsMovable);
    return m_item;
}

void GraphicsCircleTool::addPoint(int idx, const QPointF &pos)
{
    Q_ASSERT(idx < 2);

    if ( idx == 0)
        return;

    QPointF itemPos = m_item->mapFromScene(pos);
    m_item->setRadius(itemPos.x());
}

void GraphicsCircleTool::freezePoint(int idx)
{
    Q_ASSERT(idx < 2);

    if ( idx == 0)
        return;

    emit objectInserted(m_item);
    resetTool();
}

bool GraphicsCircleTool::removePoint(int idx)
{
    Q_UNUSED(idx);
    return false;
}

void GraphicsCircleTool::movePoint(int idx, const QPointF &pos)
{
    Q_ASSERT(idx == 1);

     QPointF itemPos = m_item->mapFromScene(pos);
     m_item->setRadius(itemPos.x());
}

void GraphicsCircleTool::endInsert(const QPointF &pos)
{
    Q_UNUSED(pos);
}

void GraphicsCircleTool::cancelInsert()
{
}
