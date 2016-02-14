#include "tool/graphicsbeziertool.h"
#include "item/graphicsbezieritem.h"
#include "utils/widgets/pensettingswidget.h"
#include "handle/abstractgraphicshandle.h"

#include <QAction>

GraphicsBezierTool::GraphicsBezierTool(QObject *parent):
    AbstractGraphicsInsertTool(parent),
    m_item(nullptr)
{
    QAction *action = new QAction(QIcon(":/icons/tool/graphicsbeziertool.svg"),
                           "Place a bezier curve", nullptr);
    action->setShortcut(QKeySequence("i,b"));
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

GraphicsBezierTool::~GraphicsBezierTool()
{

}

void GraphicsBezierTool::activate(const QAction *which)
{
    Q_UNUSED(which);
}

void GraphicsBezierTool::desactivate(const QAction *which)
{
    Q_UNUSED(which);
}

SchItem *GraphicsBezierTool::beginInsert(const QPointF &pos)
{
    m_item = new GraphicsBezierItem();
    m_item->setPos(pos);
    m_item->setPen(m_penSettingsWidget->pen());
    return m_item;
}

void GraphicsBezierTool::addPoint(int idx, const QPointF &pos)
{
    Q_UNUSED(idx);
    m_item->addPoint(m_item->mapFromScene(pos));
}

void GraphicsBezierTool::freezePoint(int idx, const QPointF &pos)
{
    Q_UNUSED(pos);
    Q_UNUSED(idx);
}

bool GraphicsBezierTool::removePoint(int idx, const QPointF &pos)
{
    Q_UNUSED(pos);
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

