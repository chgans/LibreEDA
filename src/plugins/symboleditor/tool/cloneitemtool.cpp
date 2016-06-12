#include "cloneitemtool.h"
#include "view/scene.h"
#include "item/item.h"
#include "command/clonecommand.h"

#include <QMouseEvent>
#include <QGraphicsColorizeEffect>
#include <QIcon>
#include <QAction>

namespace SymbolEditor
{

    CloneItemTool::CloneItemTool(Editor *editor):
        InteractiveTool (editor)
    {
        QAction *action = new QAction(QIcon::fromTheme("edit-copy"), // FIXME
                                      "<b>C</b>lone items <i>e,c</i>", nullptr);
        action->setShortcut(QKeySequence("e,c"));
        setAction(action);
        setToolGroup("interactive-tools");
    }

    Item *CloneItemTool::cloneItem(Item *item)
    {
        Item *clone = item->clone();

        QGraphicsColorizeEffect *effect = new QGraphicsColorizeEffect();
        effect->setColor(Qt::darkGray);
        effect->setStrength(.5);
        effect->setEnabled(true);
        clone->setGraphicsEffect(effect);

        clone->setSelected(false);

        scene()->addItem(clone);

        return clone;
    }

    QList<Item *> CloneItemTool::cloneItems(QList<Item *> items)
    {
        QList<Item *> clones;
        for (Item *item : items)
        {
            clones.append(cloneItem(item));
        }
        return clones;
    }

    void CloneItemTool::mousePressEvent(QMouseEvent *event)
    {
        if (m_state == ChooseDestination)
        {
            return;
        }

        m_pressPosition = event->pos();
        m_lastPosition = m_pressPosition;
    }

    void CloneItemTool::mouseMoveEvent(QMouseEvent *event)
    {
        if (m_state == ChooseOrigin)
        {
            return;
        }

        QPointF delta = event->pos() - m_lastPosition;
        m_lastPosition = event->pos();

        for (auto clone: m_clonedItems)
        {
            clone->moveBy(delta.x(), delta.y());
        }

    }

    void CloneItemTool::mouseReleaseEvent(QMouseEvent *event)
    {
        if (m_state == ChooseOrigin)
        {
            m_state = ChooseDestination;
            m_items = scene()->selectedObjects();
            m_clonedItems = cloneItems(m_items);
            return;
        }

        if (m_items.isEmpty())
        {
            resetTool();
            return;
        }

        QPointF delta = event->pos() - m_pressPosition;
        if (delta.isNull())
        {
            resetTool();
            return;
        }

        auto command = new CloneCommand;
        for (auto item : m_items)
        {
            command->itemIdList << item->data(0).value<quint64>();
        }
        command->translation = delta;
        emit commandRequested(command);

        resetTool();
    }

    void CloneItemTool::resetTool()
    {
        m_state = ChooseOrigin;
        qDeleteAll(m_clonedItems);
        m_clonedItems.clear();
        m_items.clear();
    }

    void CloneItemTool::activate()
    {
        resetTool();
    }

    void CloneItemTool::desactivate()
    {
        resetTool();
    }

    void CloneItemTool::keyPressEvent(QKeyEvent *event)
    {
        Q_UNUSED(event);
    }

    void CloneItemTool::keyReleaseEvent(QKeyEvent *event)
    {
        if (event->key() != Qt::Key_Escape)
        {
            return;
        }

        switch (m_state)
        {
            case ChooseOrigin:
                emit finished();
                break;
            case ChooseDestination:
                resetTool();
                break;
        }

        event->accept();
    }

}
