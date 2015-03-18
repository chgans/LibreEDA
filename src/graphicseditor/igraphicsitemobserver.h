#ifndef IGRAPHICSITEMOBSERVER_H
#define IGRAPHICSITEMOBSERVER_H

#include <QList>

class IGraphicsObservableItem;

class IGraphicsItemObserver
{
public:
    IGraphicsItemObserver();
    virtual ~IGraphicsItemObserver();

    virtual void itemNotification(IGraphicsObservableItem *item) = 0;

    void blockItemNotification(IGraphicsObservableItem *item = nullptr);
    void unblockItemNotification(IGraphicsObservableItem *item = nullptr);

    void addObservedItem(IGraphicsObservableItem *item);
    void removeObservedItem(IGraphicsObservableItem *item);

    QList<IGraphicsObservableItem *> observedItems() const;

private:
    bool m_operationInProgress;
    QList<IGraphicsObservableItem *> m_items;
    QList<IGraphicsObservableItem *> m_blockedItems;
    bool m_blockAllItems;

    void beginObservedItemTransaction();
    void endObserveredItemTransaction();
    void onItemNotification(IGraphicsObservableItem *item);
    friend class IGraphicsObservableItem;
};
#endif // IGRAPHICSITEMOBSERVER_H
