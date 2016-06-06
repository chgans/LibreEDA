#include "moveitemtool.h"
#include "view/scene.h"
#include "item/item.h"
#include "command/movecommand.h"

#include <QMouseEvent>
#include <QIcon>
#include <QAction>

namespace SymbolEditor
{

    MoveItemTool::MoveItemTool(QObject *parent):
        InteractiveTool (parent)
    {
        QAction *action = new QAction(QIcon::fromTheme("transform-move"),
                                      "<b>M</b>ove items <i>e,m</i>", nullptr);
        action->setShortcut(QKeySequence("e,m"));
        setAction(action);
        setToolGroup("interactive-tools");
    }

    void MoveItemTool::mousePressEvent(QMouseEvent *event)
    {
        if (m_state == ChooseDestination)
        {
            return;
        }

        m_pressPosition = event->pos();
        m_lastPosition = m_pressPosition;
    }

    void MoveItemTool::mouseMoveEvent(QMouseEvent *event)
    {
        if (m_state == ChooseOrigin)
        {
            return;
        }

        QPointF delta = event->pos() - m_lastPosition;
        m_lastPosition = event->pos();

        for (auto item: m_items)
        {
            item->moveBy(delta.x(), delta.y());
        }
    }

    void MoveItemTool::mouseReleaseEvent(QMouseEvent *event)
    {
        if (m_state == ChooseOrigin)
        {
            m_state = ChooseDestination;
            m_items = scene()->selectedObjects();
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

        auto command = new TranslateCommand;
        for (auto item : m_items)
        {
            command->itemIdList << item->data(0).value<quint64>();
        }
        command->amount = delta;
        emit commandRequested(command);

        resetTool();
    }

    void MoveItemTool::resetTool()
    {
        m_state = ChooseOrigin;
        m_items.clear();
    }

    void MoveItemTool::activate(View *view)
    {
        setView(view);
        resetTool();
    }

    void MoveItemTool::desactivate()
    {
        resetTool();
    }

    void MoveItemTool::keyPressEvent(QKeyEvent *event)
    {
        Q_UNUSED(event);
    }

    void MoveItemTool::keyReleaseEvent(QKeyEvent *event)
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
                QPointF delta = m_lastPosition - m_pressPosition;
                for (auto item: m_items)
                {
                    item->moveBy(-delta.x(), -delta.y());
                }
                resetTool();
                break;
        }

        event->accept();
    }

}
