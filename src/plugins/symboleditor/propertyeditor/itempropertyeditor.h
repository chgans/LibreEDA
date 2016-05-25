#pragma once

#include <qtpropertybrowser/qttreepropertybrowser.h>

class QtDoublePropertyManager;
class QtBoolPropertyManager;
class QtGroupPropertyManager;

namespace SymbolEditor
{

    class Item;

    class ItemPropertyEditor : public QtTreePropertyBrowser
    {
        Q_OBJECT
    public:
        explicit ItemPropertyEditor(QWidget *parent = nullptr);
        virtual ~ItemPropertyEditor();

        const Item *item() const;

    public slots:
        void setItem(const Item *item);

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

        const Item *m_item;
    };

}
