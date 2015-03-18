#include "igraphicsitemobserver.h"
#include "igraphicsobservableitem.h"

#include <QDebug>

IGraphicsItemObserver::IGraphicsItemObserver():
    m_operationInProgress(false),
    m_blockAllItems(false)
{

}

IGraphicsItemObserver::~IGraphicsItemObserver()
{
    foreach(IGraphicsObservableItem *item, m_items) {
        removeObservedItem(item);
    }
}

void IGraphicsItemObserver::blockItemNotification(IGraphicsObservableItem *item)
{
    if (item == nullptr) {
        m_blockedItems.clear();
        m_blockAllItems = true;
        return;
    }

    if (!m_items.contains(item)) {
        qWarning() << __PRETTY_FUNCTION__ << "Trying to block an item which is not in our list of observed item. Ignoring request";
        return;
    }

    if (m_blockAllItems) {
        qWarning() << __PRETTY_FUNCTION__ << "Trying to block an item while all items are currently blocked. Ignoring request";
        return;
    }

    if (!m_blockedItems.contains(item)){
        m_blockedItems.append(item);
        m_blockAllItems = false;
        return;
    }
}

void IGraphicsItemObserver::unblockItemNotification(IGraphicsObservableItem *item)
{
    if (item == nullptr) {
        m_blockedItems.clear();
        m_blockAllItems = false;
        return;
    }

    if (!m_items.contains(item)) {
        qWarning() << __PRETTY_FUNCTION__ << "Trying to unblock an item which is not in our list of observed item. Ignoring request";
        return;
    }

    if (!m_blockedItems.contains(item)) {
        qWarning() << __PRETTY_FUNCTION__ << "Trying to unblock an item which is not in our list of blocked item. Ignoring request";
        return;
    }

    if (m_blockAllItems) {
        m_blockAllItems = false;
        m_blockedItems = m_items;
        m_blockedItems.removeOne(item);
        return;
    }

    m_blockedItems.removeOne(item);
}

void IGraphicsItemObserver::beginObservedItemTransaction()
{
    Q_ASSERT(!m_operationInProgress);
    m_operationInProgress = true;
}

void IGraphicsItemObserver::addObservedItem(IGraphicsObservableItem *item)
{
    if (m_items.contains(item))
        return;

    m_items.append(item);
    if (!m_operationInProgress) {
        item->beginItemObserverTransaction();
        item->addItemObserver(this);
        item->endItemObserverTransaction();
    }
}

void IGraphicsItemObserver::removeObservedItem(IGraphicsObservableItem *item)
{
    if (!m_items.contains(item))
        return;

    m_items.removeOne(item);
    if (!m_operationInProgress) {
        item->beginItemObserverTransaction();
        item->removeItemObserver(this);
        item->endItemObserverTransaction();
    }

    if (m_blockedItems.contains(item)) {
        m_blockedItems.removeOne(item);
    }
}

void IGraphicsItemObserver::endObserveredItemTransaction()
{
    Q_ASSERT(m_operationInProgress);
    m_operationInProgress = false;
}

void IGraphicsItemObserver::onItemNotification(IGraphicsObservableItem *item)
{
    Q_ASSERT(!m_operationInProgress);
    if (m_blockAllItems || m_blockedItems.contains(item))
        return;
    itemNotification(item);
}
