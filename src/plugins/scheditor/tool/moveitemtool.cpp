#include "moveitemtool.h"
#include "schscene.h"
#include "item/schitem.h"
#include "command/placeitemcommand.h"

#include <QMouseEvent>

MoveItemTool::MoveItemTool(QObject *parent):
    InteractiveTool (parent)
{

}

void MoveItemTool::mousePressEvent(QMouseEvent *event)
{
    m_items = scene()->selectedObjects();
    m_pressPosition = event->pos();
    m_lastPosition = m_pressPosition;
}

void MoveItemTool::mouseMoveEvent(QMouseEvent *event)
{
    QPointF delta = event->pos() - m_lastPosition;
    m_lastPosition = event->pos();

    for (auto item: m_items)
    {
        item->moveBy(delta.x(), delta.y());
    }
}

void MoveItemTool::mouseReleaseEvent(QMouseEvent *event)
{
    QPointF delta = event->pos() - m_pressPosition;

    if (delta.isNull() || m_items.isEmpty())
    {
        return;
    }

    auto command = new TranslateCommand;
    for (auto item : m_items)
    {
        command->itemIdList << item->data(0).value<quint64>();
    }
    command->amount = delta;
    emit taskCompleted(command);

    m_items.clear();
}
