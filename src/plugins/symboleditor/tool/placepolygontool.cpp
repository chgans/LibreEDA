#include "tool/placepolygontool.h"
#include "item/polygonitem.h"
#include "command/placepolygoncommand.h"

#include "utils/widgets/pensettingswidget.h"
#include "utils/widgets/brushsettingswidget.h"

#include <QPolygonF>
#include <QAction>

namespace SymbolEditor
{

    PlacePolygonTool::PlacePolygonTool(Editor *editor):
        PlacementTool(editor),
        m_item(nullptr)
    {
        QAction *action = new QAction(QIcon::fromTheme("draw-polygon"), //QIcon(":/icons/tool/graphicspolygontool.svg"),
                                      "<b>P</b>lace a Poly<b>g</b>on <i>p,g</i>", nullptr);
        action->setShortcut(QKeySequence("p,g"));
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
            //m_item->setPen(pen);
        });
        m_brushSettingsWidget = new BrushSettingsWidget();
        connect(m_brushSettingsWidget, &BrushSettingsWidget::brushChanged,
                [this](const QBrush & brush)
        {
            if (!m_item)
            {
                return;
            }
            //m_item->setBrush(brush);
        });

        QList<QWidget *> widgets;
        widgets << m_penSettingsWidget << m_brushSettingsWidget;
        setTaskWidgets(widgets);
    }

    PlacePolygonTool::~PlacePolygonTool()
    {

    }

    Item *PlacePolygonTool::beginInsert(const QPointF &pos)
    {
        m_item = new PolygonItem();
        m_item->setPos(pos);
        //m_item->setPen(m_penSettingsWidget->pen());
        //m_item->setBrush(m_brushSettingsWidget->brush());
        return m_item;
    }

    void PlacePolygonTool::addPoint(int idx, const QPointF &pos)
    {
        Q_UNUSED(idx);
        Q_UNUSED(pos);
        QPointF itemPos = m_item->mapFromScene(pos);
        m_item->addPoint(itemPos);
    }

    void PlacePolygonTool::freezePoint(int idx, const QPointF &pos)
    {
        Q_UNUSED(pos);
        Q_UNUSED(idx);
    }

    bool PlacePolygonTool::removePoint(int idx, const QPointF &pos)
    {
        Q_UNUSED(pos);
        QPolygonF poly = m_item->polygon();
        QPointF point = poly[idx];
        poly.removeAt(idx);
        if (poly.count() > 1)
        {
            poly[idx - 1] = point;
            m_item->setPolygon(poly);
            return true; // Keep going
        }
        else
        {
            return false;    // Remove and delete polygon
        }
    }

    void PlacePolygonTool::movePoint(int idx, const QPointF &pos)
    {
        QPointF itemPos = m_item->mapFromScene(pos);
        m_item->movePoint(idx, itemPos);
    }

    void PlacePolygonTool::endInsert(const QPointF &pos)
    {
        Q_UNUSED(pos);

        auto command = new PlacePolygonCommand;
        command->position = m_item->pos();
        command->opacity = m_item->opacity();
        command->zValue = m_item->zValue();
        command->lineStyle = m_item->lineStyle();
        command->lineWidth = m_item->lineWidth();
        command->lineColor = m_item->lineColor();
        command->fillStyle = m_item->fillStyle();
        command->fillColor = m_item->fillColor();
        command->vertices = m_item->polygon().toList();
        emit commandRequested(command);

        delete m_item;
        m_item = nullptr;

        resetTool();
    }

    void PlacePolygonTool::cancelInsert()
    {
    }

}
