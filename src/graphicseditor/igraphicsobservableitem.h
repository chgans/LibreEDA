#ifndef IGRAPHICSOBSERVABLEITEM_H
#define IGRAPHICSOBSERVABLEITEM_H

#include <QList>

class IGraphicsItemObserver;

class IGraphicsObservableItem
{
public:
    IGraphicsObservableItem();
    virtual ~IGraphicsObservableItem();

    void addItemObserver(IGraphicsItemObserver *observer);
    void removeItemObserver(IGraphicsItemObserver *observer);

    QList<IGraphicsItemObserver *> itemObservers() const;

protected:
    void notifyObservers();

private:
    QList<IGraphicsItemObserver *> m_observers;
    bool m_transactionInProgress;
    void beginItemObserverTransaction();
    void endItemObserverTransaction();
    friend class IGraphicsItemObserver;
};

#endif // IGRAPHICSOBSERVABLEITEM_H
