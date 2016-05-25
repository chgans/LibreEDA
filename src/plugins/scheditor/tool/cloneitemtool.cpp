#include "cloneitemtool.h"
#include "scene.h"
#include "item/item.h"
#include "command/placeitemcommand.h"

#include <QMouseEvent>
#include <QGraphicsColorizeEffect>

using namespace SymbolEditor;

CloneItemTool::CloneItemTool(QObject *parent):
    InteractiveTool (parent)
{

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
    m_items = scene()->selectedObjects();
    m_pressPosition = event->pos();
    m_lastPosition = m_pressPosition;
}

void CloneItemTool::mouseMoveEvent(QMouseEvent *event)
{
    QPointF delta = event->pos() - m_lastPosition;
    m_lastPosition = event->pos();

    if (m_items.isEmpty())
    {
        return;
    }

    if (m_clonedItems.isEmpty())
    {
        m_clonedItems = cloneItems(m_items);
    }

    for (auto clone: m_clonedItems)
    {
        clone->moveBy(delta.x(), delta.y());
    }

}

void CloneItemTool::mouseReleaseEvent(QMouseEvent *event)
{
    QPointF delta = event->pos() - m_pressPosition;

    if (delta.isNull() || m_items.isEmpty())
    {
        return;
    }

    auto command = new CloneCommand;
    for (auto item : m_items)
    {
        command->itemIdList << item->data(0).value<quint64>();
    }
    command->translation = delta;
    emit taskCompleted(command);

    qDeleteAll(m_clonedItems);
    m_clonedItems.clear();
    m_items.clear();
}
