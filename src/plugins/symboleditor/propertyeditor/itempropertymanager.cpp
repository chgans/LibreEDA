#include "itempropertymanager.h"

#include <qtpropertybrowser/qtpropertymanager.h>
#include <qtpropertybrowser/qteditorfactory.h>

using namespace SymbolEditor;

ItemPropertyManager::ItemPropertyManager(QObject *parent):
    QObject(parent)
{
    m_realPropertyManager = new QtDoublePropertyManager(this);
    connect(m_realPropertyManager, &QtDoublePropertyManager::valueChanged,
            this, &ItemPropertyManager::realValueChanged);
    m_boolPropertyManager = new QtBoolPropertyManager(this);
    connect(m_boolPropertyManager, &QtBoolPropertyManager::valueChanged,
            this, &ItemPropertyManager::boolValueChanged);
}

ItemPropertyManager::~ItemPropertyManager()
{

}

void ItemPropertyManager::setBrowserFactories(QtAbstractPropertyBrowser *browser)
{
    browser->setFactoryForManager(m_realPropertyManager, new QtDoubleSpinBoxFactory);
    browser->setFactoryForManager(m_boolPropertyManager, new QtCheckBoxFactory);
}

QtProperty *ItemPropertyManager::addRealProperty(const QString &name, qreal start, qreal step, qreal end)
{
    auto property = m_realPropertyManager->addProperty(name);
    m_realPropertyManager->setMinimum(property, start);
    m_realPropertyManager->setSingleStep(property, step);
    m_realPropertyManager->setMaximum(property, end - step);
    return property;
}

QtProperty *ItemPropertyManager::addBoolProperty(const QString &name)
{
    return m_boolPropertyManager->addProperty(name);
}

void ItemPropertyManager::setPropertyValue(QtProperty *property, qreal value)
{
    m_realPropertyManager->setValue(property, value);
}

void ItemPropertyManager::setPropertyValue(QtProperty *property, bool value)
{
    m_boolPropertyManager->setValue(property, value);
}
