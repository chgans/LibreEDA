#include "tool/placerectangletool.h"
#include "item/rectangleitem.h"
#include "utils/widgets/pensettingswidget.h"
#include "utils/widgets/brushsettingswidget.h"
#include "handle/handle.h"
#include "command/placerectanglecommand.h"

#include <QAction>

namespace SymbolEditor
{

    PlaceRectangleTool::PlaceRectangleTool(QObject *parent):
        PlacementTool(parent), m_item(nullptr)
    {
        QAction *action = new QAction(QIcon::fromTheme("draw-rectangle"),//QIcon(":/icons/tool/graphicsrecttool.svg"),
                                      "<b>P</b>lace a <b>R</b>ectangle <i>p,r</i>", nullptr);
        action->setShortcut(QKeySequence("p,r"));
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

    PlaceRectangleTool::~PlaceRectangleTool()
    {

    }

    void PlaceRectangleTool::cancel()
    {
    }

    Item *PlaceRectangleTool::beginInsert(const QPointF &pos)
    {
        m_item = new RectangleItem();
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
        {
            setP1(itemPos);
        }
        else
        {
            setP2(itemPos);
        }
    }

    void PlaceRectangleTool::freezePoint(int idx, const QPointF &pos)
    {
        Q_UNUSED(pos);
        Q_ASSERT(idx < 2);

        if (idx == 0)
        {
            return;
        }

        auto command = new PlaceRectangleCommand;
        command->position = m_item->rect().topLeft();
        command->opacity = m_item->opacity();
        command->zValue = m_item->zValue();
        command->pen = m_item->pen();
        command->brush = m_item->brush();
        command->width = m_item->rect().width();
        command->height = m_item->rect().height();
        emit commandRequested(command);

        delete m_item;
        m_item = nullptr;

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
        {
            setP1(itemPos);
        }
        else
        {
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

}
