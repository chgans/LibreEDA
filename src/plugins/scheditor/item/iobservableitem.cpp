#include "item/iobservableitem.h"
#include "item/iitemobserver.h"

QList<IObservableItem *> IItemObserver::observedItems() const
{
    return m_items;
}


IObservableItem::IObservableItem():
    m_transactionInProgress(false)
{

}

IObservableItem::~IObservableItem()
{
    for (IItemObserver *observer : m_observers)
    {
        removeItemObserver(observer);
    }
}

void IObservableItem::beginItemObserverTransaction()
{
    Q_ASSERT(!m_transactionInProgress);
    m_transactionInProgress = true;
}

void IObservableItem::addItemObserver(IItemObserver *observer)
{
    if (m_observers.contains(observer))
    {
        return;
    }
    m_observers.append(observer);
    if (!m_transactionInProgress)
    {
        observer->beginObservedItemTransaction();
        observer->addObservedItem(this);
        observer->endObserveredItemTransaction();
    }
}

void IObservableItem::removeItemObserver(IItemObserver *observer)
{
    if (!m_observers.contains(observer))
    {
        return;
    }
    m_observers.removeOne(observer);
    if (!m_transactionInProgress)
    {
        observer->beginObservedItemTransaction();
        observer->removeObservedItem(this);
        observer->endObserveredItemTransaction();
    }
}

void IObservableItem::endItemObserverTransaction()
{
    Q_ASSERT(m_transactionInProgress);
    m_transactionInProgress = false;
}

QList<IItemObserver *> IObservableItem::itemObservers() const
{
    return m_observers;
}

void IObservableItem::notifyObservers()
{
    Q_ASSERT(!m_transactionInProgress);
    for (IItemObserver *observer : m_observers)
    {
        observer->onItemNotification(this);
    }
}
