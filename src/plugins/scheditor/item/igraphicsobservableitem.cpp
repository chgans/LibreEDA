#include "item/igraphicsobservableitem.h"
#include "item/igraphicsitemobserver.h"

QList<IGraphicsObservableItem *> IGraphicsItemObserver::observedItems() const
{
    return m_items;
}


IGraphicsObservableItem::IGraphicsObservableItem():
    m_transactionInProgress(false)
{

}

IGraphicsObservableItem::~IGraphicsObservableItem()
{
    for (IGraphicsItemObserver *observer: m_observers) {
        removeItemObserver(observer);
    }
}

void IGraphicsObservableItem::beginItemObserverTransaction()
{
    Q_ASSERT(!m_transactionInProgress);
    m_transactionInProgress = true;
}

void IGraphicsObservableItem::addItemObserver(IGraphicsItemObserver *observer)
{
    if (m_observers.contains(observer))
        return;
    m_observers.append(observer);
    if (!m_transactionInProgress) {
        observer->beginObservedItemTransaction();
        observer->addObservedItem(this);
        observer->endObserveredItemTransaction();
    }
}

void IGraphicsObservableItem::removeItemObserver(IGraphicsItemObserver *observer)
{
    if (!m_observers.contains(observer))
        return;
    m_observers.removeOne(observer);
    if (!m_transactionInProgress) {
        observer->beginObservedItemTransaction();
        observer->removeObservedItem(this);
        observer->endObserveredItemTransaction();
    }
}

void IGraphicsObservableItem::endItemObserverTransaction()
{
    Q_ASSERT(m_transactionInProgress);
    m_transactionInProgress = false;
}

QList<IGraphicsItemObserver *> IGraphicsObservableItem::itemObservers() const
{
    return m_observers;
}

void IGraphicsObservableItem::notifyObservers()
{
    Q_ASSERT(!m_transactionInProgress);
    for (IGraphicsItemObserver *observer: m_observers) {
        observer->onItemNotification(this);
    }
}
