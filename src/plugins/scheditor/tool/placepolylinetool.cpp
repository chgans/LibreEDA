#include "tool/placepolylinetool.h"
#include "item/polylineitem.h"

#include "utils/widgets/pensettingswidget.h"
#include "handle/handle.h"

#include <QAction>

PlacePolyineTool::PlacePolyineTool(QObject *parent):
    PlacementTool(parent), m_item(nullptr)
{
    QAction *action = new QAction(QIcon(":/icons/tool/graphicslinetool.svg"),
                                  "Place a line", nullptr);
    action->setShortcut(QKeySequence("i,l"));
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

void PlacePolyineTool::cancel()
{
}

Item *PlacePolyineTool::beginInsert(const QPointF &pos)
{
    m_item = new PolylineItem();
    m_item->setPos(pos);
    m_item->setPen(m_penSettingsWidget->pen());
    return m_item;
}

void PlacePolyineTool::addPoint(int idx, const QPointF &pos)
{
    Q_ASSERT(idx < 2);

    QPointF itemPos = m_item->mapFromScene(pos);
    if (idx == 0)
    {
        setP1(itemPos);
    }
    else
    {
        setP2(itemPos);
    }
}

void PlacePolyineTool::freezePoint(int idx, const QPointF &pos)
{
    Q_UNUSED(pos);
    if (idx == 0)
    {
        return;
    }

    emit taskCompleted(nullptr);

    delete m_item;
    m_item = nullptr;

    resetTool();
}

bool PlacePolyineTool::removePoint(int idx, const QPointF &pos)
{
    Q_UNUSED(pos);
    Q_UNUSED(idx);
    return false; // Remove and delete line
}

void PlacePolyineTool::movePoint(int idx, const QPointF &pos)
{
    Q_ASSERT(idx < 2);

    QPointF itemPos = m_item->mapFromScene(pos);
    if (idx == 0)
    {
        setP1(itemPos);
    }
    else
    {
        setP2(itemPos);
    }
}

void PlacePolyineTool::endInsert(const QPointF &pos)
{
    Q_UNUSED(pos);
}

void PlacePolyineTool::cancelInsert()
{

}

void PlacePolyineTool::setP1(const QPointF &pos)
{
    QLineF line = m_item->line();
    line.setP1(pos);
    m_item->setLine(line);
}

void PlacePolyineTool::setP2(const QPointF &pos)
{
    QLineF line = m_item->line();
    line.setP2(pos);
    m_item->setLine(line);
}
