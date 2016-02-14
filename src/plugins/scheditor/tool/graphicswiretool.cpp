#include "tool/graphicswiretool.h"
#include "item/graphicswireitem.h"

#include "utils/widgets/pensettingswidget.h"
#include "widget/wireoperationwidget.h"
#include "handle/abstractgraphicshandle.h"

#include <QAction>

GraphicsWireTool::GraphicsWireTool(QObject *parent):
    AbstractGraphicsInsertTool(parent),
    m_item(nullptr)
{
    QAction *action = new QAction(QIcon(":/icons/tool/graphicspolylinetool.svg"), // TODO: rename to wire
                               "Place a wire", nullptr);;
    action->setShortcut(QKeySequence("i,w"));
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

GraphicsWireTool::~GraphicsWireTool()
{
}

void GraphicsWireTool::activate(const QAction *which)
{
    Q_UNUSED(which);
}

void GraphicsWireTool::desactivate(const QAction *which)
{
    Q_UNUSED(which);
}

SchItem *GraphicsWireTool::beginInsert(const QPointF &pos)
{
    m_item = new GraphicsWireItem();
    m_item->setPos(pos);
    m_item->setPen(m_penSettingsWidget->pen());
    return m_item;
}

void GraphicsWireTool::addPoint(int idx, const QPointF &pos)
{
    Q_UNUSED(idx);
    m_item->addPoint(m_item->mapFromScene(pos));
}

void GraphicsWireTool::freezePoint(int idx, const QPointF &pos)
{
    Q_UNUSED(pos);
    Q_UNUSED(idx);
}

bool GraphicsWireTool::removePoint(int idx, const QPointF &pos)
{
    Q_UNUSED(pos);
    QList<QPointF> points = m_item->points();
    QPointF point = points[idx];
    points.removeAt(idx);
    if (points.count() > 1) {
        points[idx - 1] = point;
        m_item->setPoints(points);
        return true; // Keep going
    }
    else {
        return false; // remove and delete wire
    }
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
