#include "itempropertymanager.h"
#include "penpropertymanager.h"
#include "peneditorfactory.h"

#include <qtpropertybrowser/qtpropertymanager.h>
#include <qtpropertybrowser/qteditorfactory.h>

namespace SymbolEditor
{

    ItemPropertyManager::ItemPropertyManager(QObject *parent):
        QObject(parent)
    {
        m_pointManager = new QtPointFPropertyManager(this);
        connect(m_pointManager, &QtPointFPropertyManager::valueChanged,
                this, &ItemPropertyManager::pointValueChanged);

        m_realManager = new QtDoublePropertyManager(this);
        connect(m_realManager, &QtDoublePropertyManager::valueChanged,
                this, &ItemPropertyManager::realValueChanged);

        m_boolManager = new QtBoolPropertyManager(this);
        connect(m_boolManager, &QtBoolPropertyManager::valueChanged,
                this, &ItemPropertyManager::boolValueChanged);

        m_penManager = new PenPropertyManager(this);
        connect(m_penManager, &PenPropertyManager::valueChanged,
                this, &ItemPropertyManager::penValueChanged);
    }

    ItemPropertyManager::~ItemPropertyManager()
    {

    }

    void ItemPropertyManager::setBrowserFactories(QtAbstractPropertyBrowser *browser)
    {
        browser->setFactoryForManager(m_realManager, new QtDoubleSpinBoxFactory);
        browser->setFactoryForManager(m_boolManager, new QtCheckBoxFactory);
        browser->setFactoryForManager(m_pointManager->subDoublePropertyManager(), new QtDoubleSpinBoxFactory);
        auto penfactory = new PenEditorFactory(this);
        browser->setFactoryForManager(m_penManager, penfactory);
        browser->setFactoryForManager(m_penManager->subEnumManager(), penfactory->subEnumEditorFactory());
        browser->setFactoryForManager(m_penManager->subColorManager(), penfactory->subColorEditorFactory());
    }

    QtProperty *ItemPropertyManager::addRealProperty(const QString &name, qreal start, qreal step, qreal end)
    {
        auto property = m_realManager->addProperty(name);
        m_realManager->setMinimum(property, start);
        m_realManager->setSingleStep(property, step);
        m_realManager->setMaximum(property, end - step);
        return property;
    }

    QtProperty *ItemPropertyManager::addBoolProperty(const QString &name)
    {
        return m_boolManager->addProperty(name);
    }

    QtProperty *ItemPropertyManager::addPointProperty(const QString &name)
    {
        return m_pointManager->addProperty(name);
    }

    QtProperty *ItemPropertyManager::addPenProperty(const QString &name)
    {
        return m_penManager->addProperty(name);
    }

    void ItemPropertyManager::clear()
    {
        m_pointManager->clear();
        m_realManager->clear();
        m_boolManager->clear();
        m_penManager->clear();
    }

    void ItemPropertyManager::setPropertyValue(QtProperty *property, qreal value)
    {
        m_realManager->setValue(property, value);
    }

    void ItemPropertyManager::setPropertyValue(QtProperty *property, bool value)
    {
        m_boolManager->setValue(property, value);
    }

    void ItemPropertyManager::setPropertyValue(QtProperty *property, const QPointF &point)
    {
        m_pointManager->setValue(property, point);
    }

    void ItemPropertyManager::setPropertyValue(QtProperty *property, QPen pen)
    {
        m_penManager->setValue(property, pen);
    }

}
