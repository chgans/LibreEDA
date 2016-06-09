#include "itempropertyadapter.h"
#include "itempropertymanager.h"
#include "item/item.h"

#include <qtpropertybrowser/qtpropertymanager.h>

#include <QSet>

namespace SymbolEditor
{

    ItemPropertyAdapter::ItemPropertyAdapter(QObject *parent):
        QObject(parent),
        m_manager(nullptr),
        m_item(nullptr)
    {

    }

    void ItemPropertyAdapter::setManager(ItemPropertyManager *manager)
    {
        if (m_manager == manager)
        {
            return;
        }

        if (m_manager != nullptr)
        {
            deleteProperties(m_manager);
        }
        m_manager = manager;
        if (m_manager == nullptr)
        {
            return;
        }

        createProperties(m_manager);
        if (m_item != nullptr)
        {
            updateProperties(m_manager);
        }

        connect(m_manager, &ItemPropertyManager::boolValueChanged,
                this, &ItemPropertyAdapter::onBoolValueChanged);
        connect(m_manager, &ItemPropertyManager::realValueChanged,
                this, &ItemPropertyAdapter::onRealValueChanged);

    }

    void ItemPropertyAdapter::setItem(Item *item)
    {
        if (m_item == item)
        {
            return;
        }

        if (m_manager != nullptr)
        {
            deleteProperties(m_manager);
        }

        m_item = item;

        if (m_manager != nullptr && m_item != nullptr)
        {
            updateProperties(m_manager);
        }
    }

    QList<QtProperty *> ItemPropertyAdapter::properties() const
    {
        QList<QtProperty *> properties;
        properties << m_pos
                   << m_zValue
                   << m_opacity
                   << m_rotation
                   << m_xMirrored
                   << m_yMirrored
                   << m_locked
                   << m_visible
                   << m_pen;
        return properties;
    }

    Item *ItemPropertyAdapter::item() const
    {
        return m_item;
    }

    void ItemPropertyAdapter::createProperties(ItemPropertyManager *manager)
    {
        m_pos = manager->addPointProperty("Position");
        m_zValue = manager->addRealProperty("Z order", 0, 0.1, 100); // FIXME
        m_opacity = manager->addRealProperty("Opacity", 0, 0.1, 1);
        m_rotation = manager->addRealProperty("Rotation", 0, 0.1, 360);
        m_xMirrored = manager->addBoolProperty("X-Mirrored");
        m_yMirrored = manager->addBoolProperty("Y-Mirrored");
        m_locked = manager->addBoolProperty("Locked");
        m_visible = manager->addBoolProperty("Visible");
        m_pen = manager->addPenProperty("Pen");
    }

    void ItemPropertyAdapter::updateProperties(ItemPropertyManager *manager)
    {
        manager->setPropertyValue(m_pos, m_item->pos());
        manager->setPropertyValue(m_zValue, m_item->zValue());
        manager->setPropertyValue(m_opacity, m_item->opacity());
        manager->setPropertyValue(m_rotation, m_item->rotation());
        manager->setPropertyValue(m_xMirrored, m_item->isXMirrored());
        manager->setPropertyValue(m_yMirrored, m_item->isYMirrored());
        manager->setPropertyValue(m_locked, !m_item->isEnabled());
        manager->setPropertyValue(m_visible, m_item->isVisible());
        manager->setPropertyValue(m_pen, m_item->pen());
    }

    void ItemPropertyAdapter::deleteProperties(ItemPropertyManager *manager)
    {
        manager->clear();
    }

    void ItemPropertyAdapter::onBoolValueChanged(QtProperty *property, bool value)
    {
        // FIXME: emit command
    }

    void ItemPropertyAdapter::onRealValueChanged(QtProperty *property, qreal value)
    {
        // FIXME: emit command
    }

    void ItemPropertyAdapter::onPenValueChanged(QtProperty *property, qreal value)
    {
        // FIXME: emit command
    }

}
