#include "itempropertyadapter.h"
#include "itempropertymanager.h"
#include "item/item.h"

#include <qtpropertybrowser/qtpropertymanager.h>

#include <QSet>

using namespace SymbolEditor;

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

    m_item = item;

    if (m_manager != nullptr)
    {
        updateProperties(m_manager);
    }
}

QSet<QtProperty *> ItemPropertyAdapter::properties() const
{
    QSet<QtProperty *> properties;
    properties << m_xMirrored << m_yMirrored;
    return properties;
}

void ItemPropertyAdapter::createProperties(ItemPropertyManager *manager)
{
    m_xMirrored = manager->addBoolProperty("X-Mirrored");
    m_yMirrored = manager->addBoolProperty("Y-Mirrored");
}

void ItemPropertyAdapter::updateProperties(ItemPropertyManager *manager)
{
    manager->setPropertyValue(m_xMirrored, m_item->isXMirrored());
    manager->setPropertyValue(m_yMirrored, m_item->isYMirrored());
}

void ItemPropertyAdapter::deleteProperties(ItemPropertyManager *manager)
{
    // FIXME: not needed?
}

void ItemPropertyAdapter::onBoolValueChanged(QtProperty *property, bool value)
{
    // FIXME: emit command
}

void ItemPropertyAdapter::onRealValueChanged(QtProperty *property, qreal value)
{
    // FIXME: emit command
}
