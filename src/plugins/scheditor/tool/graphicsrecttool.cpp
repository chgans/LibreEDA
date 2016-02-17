#include "tool/graphicsrecttool.h"
#include "item/graphicsrectitem.h"
#include "utils/widgets/pensettingswidget.h"
#include "utils/widgets/brushsettingswidget.h"
#include "handle/abstractgraphicshandle.h"

#include <QAction>

GraphicsRectTool::GraphicsRectTool(QObject *parent):
    AbstractGraphicsInsertTool(parent), m_item(nullptr)
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

GraphicsRectTool::~GraphicsRectTool()
{

}

void GraphicsRectTool::cancel()
{
}

SchItem *GraphicsRectTool::beginInsert(const QPointF &pos)
{
    m_item = new GraphicsRectItem();
    m_item->setPos(pos);
    m_item->setPen(m_penSettingsWidget->pen());
    m_item->setBrush(m_brushSettingsWidget->brush());
    return m_item;
}

void GraphicsRectTool::addPoint(int idx, const QPointF &pos)
{
    Q_ASSERT(idx < 2);

    QPointF itemPos = m_item->mapFromScene(pos);
    if (idx == 0)
        setP1(itemPos);
    else {
        setP2(itemPos);
    }
}

void GraphicsRectTool::freezePoint(int idx, const QPointF &pos)
{
    Q_UNUSED(pos);
    Q_ASSERT(idx < 2);

    if (idx == 0)
        return;

    emit objectInserted(m_item);
    resetTool();
}

bool GraphicsRectTool::removePoint(int idx, const QPointF &pos)
{
    Q_UNUSED(pos);
    Q_UNUSED(idx);
    return false; // will remove and delete rect
}

void GraphicsRectTool::movePoint(int idx, const QPointF &pos)
{
    Q_ASSERT(idx < 2);

    QPointF itemPos = m_item->mapFromScene(pos);
    if (idx == 0)
        setP1(itemPos);
    else {
        setP2(itemPos);
    }
}

void GraphicsRectTool::endInsert(const QPointF &pos)
{
    Q_UNUSED(pos);
}

void GraphicsRectTool::cancelInsert()
{

}

void GraphicsRectTool::setP1(const QPointF &pos)
{
    m_rect.setTopLeft(pos);
    m_item->setRect(m_rect);
}

void GraphicsRectTool::setP2(const QPointF &pos)
{
    m_rect.setBottomRight(pos);
    m_item->setRect(m_rect.normalized());
}


void GraphicsRectTool::activate(const QAction *which)
{
    Q_UNUSED(which);
}

void GraphicsRectTool::desactivate(const QAction *which)
{
    Q_UNUSED(which);
}
