#include "tool/graphicslinetool.h"
#include "item/graphicslineitem.h"
#include "utils/widgets/pensettingswidget.h"
#include "handle/abstractgraphicshandle.h"

#include <QAction>

GraphicsLineTool::GraphicsLineTool(QObject *parent):
    AbstractGraphicsInsertTool(parent), m_item(nullptr)
{
    QAction *action = new QAction(QIcon(":/icons/tool/graphicslinetool.svg"),
                           "Place a line", nullptr);
    action->setShortcut(QKeySequence("i,l"));
    setAction(action);
    setToolGroup("interactive-tools");

    m_penSettingsWidget = new PenSettingsWidget();
    connect(m_penSettingsWidget, &PenSettingsWidget::penChanged,
            [this](const QPen &pen) {
        if (!m_item)
            return;
        m_item->setPen(pen);
    });

    QList<QWidget *> widgets;
    widgets << m_penSettingsWidget;
    setOptionWidgets(widgets);
}

void GraphicsLineTool::cancel()
{
}

void GraphicsLineTool::activate(const QAction *which)
{
    Q_UNUSED(which);
}

void GraphicsLineTool::desactivate(const QAction *which)
{
    Q_UNUSED(which);
}

SchItem *GraphicsLineTool::beginInsert(const QPointF &pos)
{
    m_item = new GraphicsLineItem();
    m_item->setPos(pos);
    m_item->setPen(m_penSettingsWidget->pen());
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

void GraphicsLineTool::freezePoint(int idx, const QPointF &pos)
{
    Q_UNUSED(pos);
    if (idx == 0)
        return;

    emit objectInserted(m_item);
    resetTool();
}

bool GraphicsLineTool::removePoint(int idx, const QPointF &pos)
{
    Q_UNUSED(pos);
    Q_UNUSED(idx);
    return false; // Remove and delete line
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
