#include "utils/widgets/pensettingswidget.h"
#include "utils/widgets/brushsettingswidget.h"
#include "tool/graphicspolygontool.h"
#include "item/graphicspolygonitem.h"

#include <QPolygonF>

#include <QAction>

GraphicsPolygonTool::GraphicsPolygonTool(QObject *parent):
    AbstractGraphicsInsertTool(parent),
    m_item(nullptr)
{
    QAction *action = new QAction(QIcon(":/icons/tool/graphicspolygontool.svg"),
                           "Place a rectangle", nullptr);
    action->setShortcut(QKeySequence("i,p"));
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

GraphicsPolygonTool::~GraphicsPolygonTool()
{

}

void GraphicsPolygonTool::activate(const QAction *which)
{
    Q_UNUSED(which);
}

void GraphicsPolygonTool::desactivate(const QAction *which)
{
    Q_UNUSED(which);
}

SchItem *GraphicsPolygonTool::beginInsert(const QPointF &pos)
{
    m_item = new GraphicsPolygonItem();
    m_item->setPos(pos);
    m_item->setPen(m_penSettingsWidget->pen());
    m_item->setBrush(m_brushSettingsWidget->brush());
    return m_item;
}

void GraphicsPolygonTool::addPoint(int idx, const QPointF &pos)
{
    Q_UNUSED(idx);
    Q_UNUSED(pos);
    QPointF itemPos = m_item->mapFromScene(pos);
    m_item->addPoint(itemPos);
}

void GraphicsPolygonTool::freezePoint(int idx, const QPointF &pos)
{
    Q_UNUSED(pos);
    Q_UNUSED(idx);
}

bool GraphicsPolygonTool::removePoint(int idx, const QPointF &pos)
{
    Q_UNUSED(pos);
    QPolygonF poly = m_item->polygon();
    QPointF point = poly[idx];
    poly.removeAt(idx);
    if (poly.count() > 1) {
        poly[idx - 1] = point;
        m_item->setPolygon(poly);
        return true; // Keep going
    }
    else
        return false; // Remove and delete polygon
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
