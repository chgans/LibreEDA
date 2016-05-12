#include "tool/placeellipsetool.h"
#include "item/graphicsellipseitem.h"
#include "utils/widgets/pensettingswidget.h"
#include "utils/widgets/brushsettingswidget.h"

#include <QAction>

PlaceEllipseTool::PlaceEllipseTool(QObject *parent):
    PlacementTool(parent)
{
    QAction *action = new QAction(QIcon(":/icons/tool/graphicsellipsetool.svg"),
                           "Place an ellipse", nullptr);
    action->setShortcut(QKeySequence("i,e"));
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

PlaceEllipseTool::~PlaceEllipseTool()
{

}

void PlaceEllipseTool::activate(const QAction *which)
{
    Q_UNUSED(which);
}

void PlaceEllipseTool::desactivate(const QAction *which)
{
    Q_UNUSED(which);
}

SchItem *PlaceEllipseTool::beginInsert(const QPointF &pos)
{
    m_item = new GraphicsEllipseItem();
    m_item->setPos(pos);
    m_item->setPen(m_penSettingsWidget->pen());
    m_item->setBrush(m_brushSettingsWidget->brush());
    return m_item;
}

void PlaceEllipseTool::addPoint(int idx, const QPointF &pos)
{
    Q_ASSERT(idx < 3);

    if ( idx == 0)
        return;

    QPointF itemPos = m_item->mapFromScene(pos);

    if (idx == 1) {
        QLineF radius(QPointF(0, 0), itemPos);
        m_item->setXRadius(radius.length());
        m_item->setYRadius(radius.length()/2.0);
        m_item->setRotation(-radius.angle());
    }
    else
        m_item->setYRadius(qAbs(itemPos.y()));
}

void PlaceEllipseTool::freezePoint(int idx, const QPointF &pos)
{
    Q_UNUSED(pos);
    Q_ASSERT(idx < 3);

    if ( idx != 2)
        return;

    emit objectInserted(m_item);
    resetTool();
}

bool PlaceEllipseTool::removePoint(int idx, const QPointF &pos)
{
    if (idx == 2) {
        QPointF itemPos = m_item->mapFromScene(pos);
        m_item->setYRadius(qAbs(itemPos.y()));
        return true; // Keep going
    }
    else
        return false; // Remove and delete ellipse
}

void PlaceEllipseTool::movePoint(int idx, const QPointF &pos)
{
    Q_ASSERT(idx < 3);

    if ( idx == 0)
        return;

    if (idx == 1) {
        m_item->setRotation(0.0);
        QPointF itemPos = m_item->mapFromScene(pos);
        QLineF radius(QPointF(0, 0), itemPos);
        m_item->setXRadius(radius.length());
        m_item->setYRadius(radius.length()/2.0);
        m_item->setRotation(-radius.angle());
    }
    else {
        QPointF itemPos = m_item->mapFromScene(pos);
        m_item->setYRadius(qAbs(itemPos.y()));
    }
}

void PlaceEllipseTool::endInsert(const QPointF &pos)
{
    Q_UNUSED(pos);
}

void PlaceEllipseTool::cancelInsert()
{
}
