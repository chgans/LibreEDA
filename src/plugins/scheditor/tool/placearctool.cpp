#include "tool/placearctool.h"
#include "item/graphicsarcitem.h"
#include "utils/widgets/pensettingswidget.h"
#include "utils/widgets/brushsettingswidget.h"

#include <QAction>

PlaceArcTool::PlaceArcTool(QObject *parent):
    PlacementTool(parent)
{
    QAction *action = new QAction(QIcon(":/icons/tool/graphicsarctool.svg"),
                           "Place an arc", nullptr);
    action->setShortcut(QKeySequence("i,a"));
    setAction(action);
    setToolGroup("interactive-tools");

    m_penSettingsWidget = new PenSettingsWidget();
    connect(m_penSettingsWidget, &PenSettingsWidget::penChanged,
            [this](const QPen &pen) {
        if (!m_item)
            return;
        m_item->setPen(pen);
    });
    m_brushSettingsWidget = new BrushSettingsWidget();
    connect(m_brushSettingsWidget, &BrushSettingsWidget::brushChanged,
            [this](const QBrush &brush) {
        if (!m_item)
            return;
        m_item->setBrush(brush);
    });

    QList<QWidget *> widgets;
    widgets << m_penSettingsWidget << m_brushSettingsWidget;
    setOptionWidgets(widgets);
}

PlaceArcTool::~PlaceArcTool()
{

}

void PlaceArcTool::activate(const QAction *which)
{
    Q_UNUSED(which);
}

void PlaceArcTool::desactivate(const QAction *which)
{
    Q_UNUSED(which);
}

SchItem *PlaceArcTool::beginInsert(const QPointF &pos)
{
    m_item = new GraphicsArcItem();
    m_item->setPos(pos);
    m_item->setPen(m_penSettingsWidget->pen());
    m_item->setBrush(m_brushSettingsWidget->brush());
    return m_item;
}

void PlaceArcTool::addPoint(int idx, const QPointF &pos)
{
    movePoint(idx, pos);
}

void PlaceArcTool::freezePoint(int idx, const QPointF &pos)
{
    Q_UNUSED(pos);

    if (idx != 4)
        return;
    emit objectInserted(m_item);
    resetTool();
}

bool PlaceArcTool::removePoint(int idx, const QPointF &pos)
{
    int handleId;
    switch (idx) {
    case 0:
    case 1:
        return false; // Remove and delete Arc
    case 2:
        handleId = GraphicsArcItem::XRadiusHandle;
        break;
    case 3:
        handleId = GraphicsArcItem::YRadiusHandle;
        break;
    case 4:
        m_item->setSpanAngle(0);
        handleId = GraphicsArcItem::StartAngleHandle;
        break;
    default:
        // Fail loudly
        Q_ASSERT(idx < 4);
        return false;
    }
    QPointF p = m_item->mapFromScene(pos);
    m_item->handleAt(handleId)->setPos(p);
    return true; // Keep going
}

void PlaceArcTool::movePoint(int idx, const QPointF &pos)
{
    QPointF p = m_item->mapFromScene(pos);
    switch (idx) {
    case 0:
        return;
    case 1:
        m_item->setXRadius(p.x());
        m_item->setYRadius(p.x());
        break;
    case 2:
        m_item->setYRadius(p.y());
        break;
    case 3:
        m_item->handleAt(GraphicsArcItem::StartAngleHandle)->setPos(p);
        m_item->handleAt(GraphicsArcItem::SpanAngleHandle)->setPos(p);
        break;
    case 4:
        m_item->handleAt(GraphicsArcItem::SpanAngleHandle)->setPos(p);
        break;
    default:
        // Fail loudly
        Q_ASSERT(idx < 5);
        return;
    }
}

void PlaceArcTool::endInsert(const QPointF &pos)
{
    Q_UNUSED(pos);
}

void PlaceArcTool::cancelInsert()
{

}
