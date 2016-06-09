#include "tool/placecircletool.h"
#include "item/circleitem.h"
#include "utils/widgets/pensettingswidget.h"
#include "utils/widgets/brushsettingswidget.h"
#include "command/placecirclecommand.h"

#include <QAction>

namespace SymbolEditor
{

    PlaceCircleTool::PlaceCircleTool(QObject *parent):
        PlacementTool(parent)
    {
        QAction *action = new QAction(QIcon::fromTheme("draw-circle"), //QIcon(":/icons/tool/graphicscircletool.svg"),
                                      "<b>P</b>lace a <b>C</b>ircle <i>p,c</i>", nullptr);
        action->setShortcut(QKeySequence("p,c"));
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
        m_brushSettingsWidget = new BrushSettingsWidget();
        connect(m_brushSettingsWidget, &BrushSettingsWidget::brushChanged,
                [this](const QBrush & brush)
        {
            if (!m_item)
            {
                return;
            }
            m_item->setBrush(brush);
        });

        QList<QWidget *> widgets;
        widgets << m_penSettingsWidget << m_brushSettingsWidget;
        setTaskWidgets(widgets);
    }

    PlaceCircleTool::~PlaceCircleTool()
    {
    }

    Item *PlaceCircleTool::beginInsert(const QPointF &pos)
    {
        m_item = new CircleItem();
        m_item->setPos(pos);
        m_item->setPen(m_penSettingsWidget->pen());
        m_item->setBrush(m_brushSettingsWidget->brush());
        return m_item;
    }

    void PlaceCircleTool::addPoint(int idx, const QPointF &pos)
    {
        Q_ASSERT(idx < 2);

        if (idx == 0)
        {
            return;
        }

        QPointF itemPos = m_item->mapFromScene(pos);
        m_item->setRadius(qAbs(itemPos.x()));
    }

    void PlaceCircleTool::freezePoint(int idx, const QPointF &pos)
    {
        Q_UNUSED(pos);
        Q_ASSERT(idx < 2);

        if (idx == 0)
        {
            return;
        }

        auto command = new PlaceCircleCommand;
        command->position = m_item->pos();
        command->opacity = m_item->opacity();
        command->zValue = m_item->zValue();
        command->pen = m_item->pen();
        command->brush = m_item->brush();
        command->center = QPointF(0, 0);
        command->radius = m_item->radius();
        emit commandRequested(command);

        delete m_item;
        m_item = nullptr;

        resetTool();
    }

    bool PlaceCircleTool::removePoint(int idx, const QPointF &pos)
    {
        Q_UNUSED(pos);
        Q_UNUSED(idx);
        return false; // Remove and delete circles
    }

    void PlaceCircleTool::movePoint(int idx, const QPointF &pos)
    {
        Q_ASSERT(idx == 1);

        QLineF vector(m_item->pos(), pos);
        m_item->setRadius(vector.length());
    }

    void PlaceCircleTool::endInsert(const QPointF &pos)
    {
        Q_UNUSED(pos);
    }

    void PlaceCircleTool::cancelInsert()
    {
    }

}
