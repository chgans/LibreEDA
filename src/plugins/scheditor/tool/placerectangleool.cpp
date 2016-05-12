#include "tool/placerectangletool.h"
#include "item/graphicsrectitem.h"
#include "utils/widgets/pensettingswidget.h"
#include "utils/widgets/brushsettingswidget.h"
#include "handle/abstractgraphicshandle.h"

#include <QAction>

PlaceRectangleTool::PlaceRectangleTool(QObject *parent):
    PlacementTool(parent), m_item(nullptr)
{
    QAction *action = new QAction(QIcon(":/icons/tool/graphicsrecttool.svg"),
                           "Place a rectangle", nullptr);
    action->setShortcut(QKeySequence("i,r"));
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

PlaceRectangleTool::~PlaceRectangleTool()
{

}

void PlaceRectangleTool::cancel()
{
}

SchItem *PlaceRectangleTool::beginInsert(const QPointF &pos)
{
    m_item = new GraphicsRectItem();
    m_item->setPos(pos);
    m_item->setRect(QRect());
    m_item->setPen(m_penSettingsWidget->pen());
    m_item->setBrush(m_brushSettingsWidget->brush());
    return m_item;
}

void PlaceRectangleTool::addPoint(int idx, const QPointF &pos)
{
    Q_ASSERT(idx < 2);

    QPointF itemPos = m_item->mapFromScene(pos);
    if (idx == 0)
        setP1(itemPos);
    else {
        setP2(itemPos);
    }
}

void PlaceRectangleTool::freezePoint(int idx, const QPointF &pos)
{
    Q_UNUSED(pos);
    Q_ASSERT(idx < 2);

    if (idx == 0)
        return;

    emit objectInserted(m_item);
    resetTool();
}

bool PlaceRectangleTool::removePoint(int idx, const QPointF &pos)
{
    Q_UNUSED(pos);
    Q_UNUSED(idx);
    return false; // will remove and delete rect
}

void PlaceRectangleTool::movePoint(int idx, const QPointF &pos)
{
    Q_ASSERT(idx < 2);

    QPointF itemPos = m_item->mapFromScene(pos);
    if (idx == 0)
        setP1(itemPos);
    else {
        setP2(itemPos);
    }
}

void PlaceRectangleTool::endInsert(const QPointF &pos)
{
    Q_UNUSED(pos);
}

void PlaceRectangleTool::cancelInsert()
{

}

void PlaceRectangleTool::setP1(const QPointF &pos)
{
    m_rect.setTopLeft(pos);
    m_rect.setBottomRight(pos);
    m_item->setRect(m_rect);
}

void PlaceRectangleTool::setP2(const QPointF &pos)
{
    m_rect.setBottomRight(pos);
    m_item->setRect(m_rect.normalized());
}


void PlaceRectangleTool::activate(const QAction *which)
{
    Q_UNUSED(which);
}

void PlaceRectangleTool::desactivate(const QAction *which)
{
    Q_UNUSED(which);
}
