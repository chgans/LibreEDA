#include "itempropertyeditor.h"
#include "itempropertyadapter.h"
#include "itempropertymanager.h"
#include "circlepropertyadapter.h"
#include "item/item.h"

// See https://github.com/qtproject/qttools/blob/dev/src/designer/src/
// - lib/sdk/abstractpropertyeditor.h
// - lib/shared/qdesigner_propertyeditor_p.h
// - components/propertyeditor/propertyeditor.h
#include <qtpropertybrowser/qtpropertymanager.h>
#include <qtpropertybrowser/qteditorfactory.h>
#include <qtpropertybrowser/qttreepropertybrowser.h>
#include <qtpropertybrowser/qtbuttonpropertybrowser.h>
#include <qtpropertybrowser/qtgroupboxpropertybrowser.h>

#include <QVBoxLayout>

namespace SymbolEditor
{

    ItemPropertyEditor::ItemPropertyEditor(QWidget *parent):
        QWidget(parent), m_item(nullptr)
    {
        setLayout(new QVBoxLayout);
        // TODO: setBrowser();
        m_browser = new QtTreePropertyBrowser();
        layout()->addWidget(m_browser);
        layout()->setMargin(0);
        // TODO: setManager();
        m_manager = new ItemPropertyManager(this);
        m_manager->setBrowserFactories(m_browser);
        // TODO: setAdapterForItemType();
        m_adapter = new CirclePropertyAdapter(this);
        m_adapter->setManager(m_manager);

        //setupPropertyManagers();
        //setupProperties();

        // TODO: Use object pool to collect ItemPropertyAdapter objects

    }

    ItemPropertyEditor::~ItemPropertyEditor()
    {

    }

    Item *ItemPropertyEditor::item() const
    {
        return m_item;
    }

    void ItemPropertyEditor::setItem(Item *item)
    {
        if (m_item == item)
        {
            return;
        }

        m_browser->clear();
        m_item = item;

        if (m_item == nullptr)
        {
            return;
        }

        m_adapter->setItem(m_item);
        for (auto property: m_adapter->properties())
        {
            m_browser->addProperty(property);
        }
    }

#if 0
    // TODO: based on item type, delegate additional property groups to be added and update of tehir values
    void ItemPropertyEditor::setItem(const Item *item)
    {
        if (item == m_item)
        {
            if (m_item != nullptr)
            {
                updatePropertyManagerValues();
            }
            return;
        }

        if (m_item == nullptr)
        {
            m_browser->addProperty(m_itemGroupProperty);
        }

        m_item = item;

        if (m_item == nullptr)
        {
            m_browser->removeProperty(m_itemGroupProperty);
            return;
        }

        updatePropertyManagerValues();

        // NEW
        // m_adpaterMap[item->type()]->setItem();
    }
#endif

    void ItemPropertyEditor::updatePropertyManagerValues()
    {
        disconnectManagers();
        m_realPropertyManager->setValue(m_opacityProperty, m_item->opacity());
        m_realPropertyManager->setValue(m_rotationProperty, m_item->rotation());
        m_realPropertyManager->setValue(m_zValueProperty, m_item->zValue());
        m_booleanPropertyManager->setValue(m_lockedProperty, !m_item->isEnabled());
        m_booleanPropertyManager->setValue(m_visibleProperty, m_item->isVisible());
        m_booleanPropertyManager->setValue(m_xMirroredProperty, m_item->isXMirrored());
        m_booleanPropertyManager->setValue(m_yMirroredProperty, m_item->isYMirrored());
        connectManagers();
    }

    void ItemPropertyEditor::connectManagers()
    {
        connect(m_realPropertyManager, &QtDoublePropertyManager::propertyChanged,
                this, &ItemPropertyEditor::onRealPropertyChanged);
        connect(m_booleanPropertyManager, &QtBoolPropertyManager::propertyChanged,
                this, &ItemPropertyEditor::onBooleanPropertyChanged);
    }

    void ItemPropertyEditor::disconnectManagers()
    {
        m_booleanPropertyManager->disconnect(this);
        m_realPropertyManager->disconnect(this);
    }

    void ItemPropertyEditor::onRealPropertyChanged(QtProperty *property)
    {
        qreal value = m_realPropertyManager->value(property);
        if (property == m_opacityProperty)
        {
            emit opacityChanged(value);
        }
        else if (property == m_rotationProperty)
        {
            emit rotationChanged(value);
        }
        else if (property == m_zValueProperty)
        {
            emit zValueChanged(value);
        }
    }

    void ItemPropertyEditor::onBooleanPropertyChanged(QtProperty *property)
    {
        bool value = m_booleanPropertyManager->value(property);
        if (property == m_lockedProperty)
        {
            emit lockStateChanged(value);
        }
        else if (property == m_visibleProperty)
        {
            emit visibilityChanged(value);
        }
        else if (property == m_xMirroredProperty)
        {
            emit xMirroringChanged(value);
        }
        else if (property == m_yMirroredProperty)
        {
            emit yMirroringChanged(value);
        }
    }

    void ItemPropertyEditor::setupProperties()
    {
        m_itemGroupProperty = m_groupPropertyManager->addProperty("Drawing Item");
        m_opacityProperty = m_realPropertyManager->addProperty("Opacity");
        m_itemGroupProperty->addSubProperty(m_opacityProperty);
        m_realPropertyManager->setMinimum(m_opacityProperty, 0.0);
        m_realPropertyManager->setMaximum(m_opacityProperty, 1.0);
        m_realPropertyManager->setSingleStep(m_opacityProperty, 0.1);
        m_rotationProperty = m_realPropertyManager->addProperty("Rotation");
        m_itemGroupProperty->addSubProperty(m_rotationProperty);
        m_realPropertyManager->setMinimum(m_rotationProperty, 0.0);
        m_realPropertyManager->setMaximum(m_rotationProperty, 360.0);
        m_realPropertyManager->setSingleStep(m_rotationProperty, 5.0);
        m_zValueProperty = m_realPropertyManager->addProperty("Z Value");
        m_itemGroupProperty->addSubProperty(m_zValueProperty);
        m_realPropertyManager->setMinimum(m_zValueProperty, 0.0);
        //m_realPropertyManager->setMaximum(zValueProperty, 0.0);
        //m_realPropertyManager->setSingleStep(zValueProperty, 0.0);
        m_lockedProperty = m_booleanPropertyManager->addProperty("Locked");
        m_itemGroupProperty->addSubProperty(m_lockedProperty);
        m_visibleProperty = m_booleanPropertyManager->addProperty("Visible");
        m_itemGroupProperty->addSubProperty(m_visibleProperty);
        m_xMirroredProperty = m_booleanPropertyManager->addProperty("X Mirrored");
        m_itemGroupProperty->addSubProperty(m_xMirroredProperty);
        m_yMirroredProperty = m_booleanPropertyManager->addProperty("Y Mirrored");
        m_itemGroupProperty->addSubProperty(m_yMirroredProperty);
        // TBD: Pen and brush (per item)


        // NEW
        // m_adpater->setupPropertyManager();
    }

    void ItemPropertyEditor::setupPropertyManagers()
    {
        m_groupPropertyManager = new QtGroupPropertyManager(this);
        m_realPropertyManager = new QtDoublePropertyManager(this);
        m_browser->setFactoryForManager(m_realPropertyManager, new QtDoubleSpinBoxFactory);
        m_booleanPropertyManager = new QtBoolPropertyManager(this);
        m_browser->setFactoryForManager(m_booleanPropertyManager, new QtCheckBoxFactory);

        // NEW
        // m_manager->setupPropertyBrowser(m_browser);
    }

}
