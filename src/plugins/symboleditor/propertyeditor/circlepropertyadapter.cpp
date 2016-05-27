#include "circlepropertyadapter.h"
#include "itempropertymanager.h"
#include "item/circleitem.h"

#include <qtpropertybrowser/qtpropertymanager.h>

#include <QSet>

using namespace SymbolEditor;

CirclePropertyAdapter::CirclePropertyAdapter(QObject *parent):
    ItemPropertyAdapter(parent)
{

}

QList<QtProperty *> CirclePropertyAdapter::properties() const
{
    auto properties = ItemPropertyAdapter::properties();
    properties << m_center << m_radius; // << m_pen << m_brush;
    return properties;
}

void CirclePropertyAdapter::createProperties(ItemPropertyManager *manager)
{
    ItemPropertyAdapter::createProperties(manager);
    m_center = manager->addPointProperty("Center");
    m_radius = manager->addRealProperty("Radius", 0, 0.1, std::numeric_limits<qreal>::max());
}

void CirclePropertyAdapter::updateProperties(ItemPropertyManager *manager)
{
    ItemPropertyAdapter::updateProperties(manager);
    auto circle = static_cast<CircleItem*>(item());
    manager->setPropertyValue(m_center, circle->pos());
    manager->setPropertyValue(m_radius, circle->radius());
}

void CirclePropertyAdapter::deleteProperties(ItemPropertyManager *manager)
{
    // FIXME: not needed?
}

void CirclePropertyAdapter::onBoolValueChanged(QtProperty *property, bool value)
{
    // FIXME: emit command
}

void CirclePropertyAdapter::onRealValueChanged(QtProperty *property, qreal value)
{
    // FIXME: emit command
}
