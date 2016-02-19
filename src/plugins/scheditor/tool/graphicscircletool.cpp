#include "tool/graphicscircletool.h"
#include "item/graphicscircleitem.h"
#include "utils/widgets/pensettingswidget.h"
#include "utils/widgets/brushsettingswidget.h"

#include <QAction>

GraphicsCircleTool::GraphicsCircleTool(QObject *parent):
    AbstractGraphicsInsertTool(parent)
{
    QAction *action = new QAction(QIcon(":/icons/tool/graphicscircletool.svg"),
                                  "Place a circle", nullptr);
    action->setShortcut(QKeySequence("i,c"));
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

GraphicsCircleTool::~GraphicsCircleTool()
{
}

void GraphicsCircleTool::activate(const QAction *which)
{
    Q_UNUSED(which);
}

void GraphicsCircleTool::desactivate(const QAction *which)
{
    Q_UNUSED(which);
}

SchItem *GraphicsCircleTool::beginInsert(const QPointF &pos)
{
    m_item = new GraphicsCircleItem();
    m_item->setPos(pos);
    m_item->setPen(m_penSettingsWidget->pen());
    m_item->setBrush(m_brushSettingsWidget->brush());
    return m_item;
}

void GraphicsCircleTool::addPoint(int idx, const QPointF &pos)
{
    Q_ASSERT(idx < 2);

    if ( idx == 0)
        return;

    QPointF itemPos = m_item->mapFromScene(pos);
    m_item->setRadius(qAbs(itemPos.x()));
}

void GraphicsCircleTool::freezePoint(int idx, const QPointF &pos)
{
    Q_UNUSED(pos);
    Q_ASSERT(idx < 2);

    if ( idx == 0)
        return;

    emit objectInserted(m_item);
    resetTool();
}

bool GraphicsCircleTool::removePoint(int idx, const QPointF &pos)
{
    Q_UNUSED(pos);
    Q_UNUSED(idx);
    return false; // Remove and delete circles
}

void GraphicsCircleTool::movePoint(int idx, const QPointF &pos)
{
    Q_ASSERT(idx == 1);

     QLineF vector(m_item->pos(), pos);
     m_item->setRadius(vector.length());
}

void GraphicsCircleTool::endInsert(const QPointF &pos)
{
    Q_UNUSED(pos);
}

void GraphicsCircleTool::cancelInsert()
{
}
