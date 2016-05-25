#ifndef IGRAPHICSOBSERVABLEITEM_H
#define IGRAPHICSOBSERVABLEITEM_H

#include <QList>

namespace SymbolEditor
{

    class IItemObserver;

    class IObservableItem
    {
    public:
        IObservableItem();
        virtual ~IObservableItem();

        void addItemObserver(IItemObserver *observer);
        void removeItemObserver(IItemObserver *observer);

        QList<IItemObserver *> itemObservers() const;

    protected:
        void notifyObservers();

    private:
        QList<IItemObserver *> m_observers;
        bool m_transactionInProgress;
        void beginItemObserverTransaction();
        void endItemObserverTransaction();
        friend class IItemObserver;
    };

}

#endif // IGRAPHICSOBSERVABLEITEM_H
