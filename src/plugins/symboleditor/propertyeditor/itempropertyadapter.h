#pragma once

#include <QObject>

class QtProperty;

namespace SymbolEditor
{

    class UndoCommand;
    class Item;
    class ItemPropertyManager;

    class ItemPropertyAdapter : public QObject
    {
        Q_OBJECT

    public:
        explicit ItemPropertyAdapter(QObject *parent = 0);

        void setManager(ItemPropertyManager *manager);
        void setItem(Item *item);

        virtual QList<QtProperty *> properties() const;

    signals:
        void taskCompleted(UndoCommand *command);

    public slots:

    protected:
        Item *item() const;
        virtual void createProperties(ItemPropertyManager *manager);
        virtual void updateProperties(ItemPropertyManager *manager);
        virtual void deleteProperties(ItemPropertyManager *manager);

    protected slots:
        void onBoolValueChanged(QtProperty *property, bool value);
        void onRealValueChanged(QtProperty *property, qreal value);
        void onPenValueChanged(QtProperty *property, qreal value);

    private:
        ItemPropertyManager *m_manager;
        Item *m_item;

        QtProperty *m_pos;
        QtProperty *m_zValue;
        QtProperty *m_opacity;
        QtProperty *m_rotation;
        QtProperty *m_xMirrored;
        QtProperty *m_yMirrored;
        QtProperty *m_locked;
        QtProperty *m_visible;
        QtProperty *m_pen;
    };

}
