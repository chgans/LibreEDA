#pragma once

#include <QList>

namespace SymbolEditor
{

    class IObservableItem;

    class IItemObserver
    {
    public:
        IItemObserver();
        virtual ~IItemObserver();

        virtual void itemNotification(IObservableItem *item) = 0;

        void blockItemNotification(IObservableItem *item = nullptr);
        void unblockItemNotification(IObservableItem *item = nullptr);

        void addObservedItem(IObservableItem *item);
        void removeObservedItem(IObservableItem *item);

        QList<IObservableItem *> observedItems() const;

    private:
        bool m_operationInProgress;
        QList<IObservableItem *> m_items;
        QList<IObservableItem *> m_blockedItems;
        bool m_blockAllItems;

        void beginObservedItemTransaction();
        void endObserveredItemTransaction();
        void onItemNotification(IObservableItem *item);
        friend class IObservableItem;
    };

}
