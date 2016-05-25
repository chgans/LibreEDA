#include "tool/placebeziertool.h"
#include "item/bezieritem.h"
#include "utils/widgets/pensettingswidget.h"
#include "handle/handle.h"

#include <QAction>

using namespace SymbolEditor;

PlaceBezierTool::PlaceBezierTool(QObject *parent):
    PlacementTool(parent),
    m_item(nullptr)
{
    QAction *action = new QAction(QIcon(":/icons/tool/graphicsbeziertool.svg"),
                                  "Place a bezier curve", nullptr);
    action->setShortcut(QKeySequence("i,b"));
    setAction(action);
    setToolGroup("interactive-tools");

    m_penSettingsWidget = new PenSettingsWidget();
    connect(m_penSettingsWidget, &PenSettingsWidget::penChanged,
            [this](const QPen & pen)
    {
        if (!m_item)
        {
            return;
        }
        m_item->setPen(pen);
    });

    QList<QWidget *> widgets;
    widgets << m_penSettingsWidget;
    setTaskWidgets(widgets);
}

PlaceBezierTool::~PlaceBezierTool()
{

}

Item *PlaceBezierTool::beginInsert(const QPointF &pos)
{
    m_item = new BezierItem();
    m_item->setPos(pos);
    m_item->setPen(m_penSettingsWidget->pen());
    return m_item;
}

void PlaceBezierTool::addPoint(int idx, const QPointF &pos)
{
    Q_UNUSED(idx);
    m_item->addPoint(m_item->mapFromScene(pos));
}

void PlaceBezierTool::freezePoint(int idx, const QPointF &pos)
{
    Q_UNUSED(pos);
    Q_UNUSED(idx);
}

bool PlaceBezierTool::removePoint(int idx, const QPointF &pos)
{
    Q_UNUSED(pos);
    QPointF oldPos = m_item->pointAt(idx);
    m_item->removePoint(idx);
    if (m_item->pointCount() > 1)
    {
        m_item->movePoint(idx - 1, oldPos);
        return true; // Keep going
    }
    else
    {
        return false; // remove and delete bezier
    }
    return false;
}

void PlaceBezierTool::movePoint(int idx, const QPointF &pos)
{
    m_item->movePoint(idx, m_item->mapFromScene(pos));
}

void PlaceBezierTool::endInsert(const QPointF &pos)
{
    Q_UNUSED(pos);

    emit taskCompleted(nullptr);

    delete m_item;
    m_item = nullptr;

    resetTool();
}

void PlaceBezierTool::cancelInsert()
{

}
