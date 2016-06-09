#include "item/iitemobserver.h"
#include "item/iobservableitem.h"

namespace SymbolEditor
{

    IItemObserver::IItemObserver():
        m_operationInProgress(false),
        m_blockAllItems(false)
    {

    }

    IItemObserver::~IItemObserver()
    {
        for (IObservableItem *item : m_items)
        {
            removeObservedItem(item);
        }
    }

    void IItemObserver::blockItemNotification(IObservableItem *item)
    {
        if (item == nullptr)
        {
            m_blockedItems.clear();
            m_blockAllItems = true;
            return;
        }

        Q_ASSERT(!m_items.contains(item));

        if (m_blockAllItems)
        {
            return;
        }

        if (!m_blockedItems.contains(item))
        {
            m_blockedItems.append(item);
            m_blockAllItems = false;
            return;
        }
    }

    void IItemObserver::unblockItemNotification(IObservableItem *item)
    {
        if (item == nullptr)
        {
            m_blockedItems.clear();
            m_blockAllItems = false;
            return;
        }

        Q_ASSERT(!m_items.contains(item));
        Q_ASSERT(!m_blockedItems.contains(item));

        if (m_blockAllItems)
        {
            m_blockAllItems = false;
            m_blockedItems = m_items;
            m_blockedItems.removeOne(item);
            return;
        }

        m_blockedItems.removeOne(item);
    }

    void IItemObserver::beginObservedItemTransaction()
    {
        Q_ASSERT(!m_operationInProgress);
        m_operationInProgress = true;
    }

    void IItemObserver::addObservedItem(IObservableItem *item)
    {
        if (m_items.contains(item))
        {
            return;
        }

        m_items.append(item);
        if (!m_operationInProgress)
        {
            item->beginItemObserverTransaction();
            item->addItemObserver(this);
            item->endItemObserverTransaction();
        }
    }

    void IItemObserver::removeObservedItem(IObservableItem *item)
    {
        if (!m_items.contains(item))
        {
            return;
        }

        m_items.removeOne(item);
        if (!m_operationInProgress)
        {
            item->beginItemObserverTransaction();
            item->removeItemObserver(this);
            item->endItemObserverTransaction();
        }

        if (m_blockedItems.contains(item))
        {
            m_blockedItems.removeOne(item);
        }
    }

    void IItemObserver::endObserveredItemTransaction()
    {
        Q_ASSERT(m_operationInProgress);
        m_operationInProgress = false;
    }

    void IItemObserver::onItemNotification(IObservableItem *item)
    {
        Q_ASSERT(!m_operationInProgress);
        if (m_blockAllItems || m_blockedItems.contains(item))
        {
            return;
        }
        itemNotification(item);
    }

}
