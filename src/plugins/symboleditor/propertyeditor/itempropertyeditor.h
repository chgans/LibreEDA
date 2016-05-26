#pragma once

#include <QWidget>

class QtDoublePropertyManager;
class QtBoolPropertyManager;
class QtGroupPropertyManager;
class QtAbstractPropertyBrowser;
class QtProperty;

namespace SymbolEditor
{

    class Item;
    class ItemPropertyManager;
    class ItemPropertyAdapter;

    class ItemPropertyEditor : public QWidget
    {
        Q_OBJECT
    public:
        explicit ItemPropertyEditor(QWidget *parent = nullptr);
        virtual ~ItemPropertyEditor();

        Item *item() const;

    public slots:
        void setItem(Item *item);

    signals:
        void opacityChanged(qreal opacity);
        void rotationChanged(qreal rotation);
        void zValueChanged(qreal zValue);
        void lockStateChanged(bool locked);
        void visibilityChanged(bool visible);
        void xMirroringChanged(bool mirrored);
        void yMirroringChanged(bool mirrored);

    protected:
        void setupProperties();
        void setupPropertyManagers();
        void updatePropertyManagerValues();
        void connectManagers();
        void disconnectManagers();

    protected slots:
        void onRealPropertyChanged(QtProperty *property);
        void onBooleanPropertyChanged(QtProperty *property);

    private:
        QtAbstractPropertyBrowser *m_browser;
        ItemPropertyManager *m_manager;
        ItemPropertyAdapter *m_adapter;

        QtGroupPropertyManager *m_groupPropertyManager;
        QtDoublePropertyManager *m_realPropertyManager;
        QtBoolPropertyManager *m_booleanPropertyManager;

        QtProperty *m_itemGroupProperty;
        QtProperty *m_opacityProperty;
        QtProperty *m_rotationProperty;
        QtProperty *m_zValueProperty;
        QtProperty *m_lockedProperty;
        QtProperty *m_visibleProperty;
        QtProperty *m_xMirroredProperty;
        QtProperty *m_yMirroredProperty;

        Item *m_item;
    };

}
