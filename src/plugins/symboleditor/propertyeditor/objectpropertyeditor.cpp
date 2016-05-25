#include "objectpropertyeditor.h"
#include "penpropertymanager.h"
#include "peneditorfactory.h"

#include "qtpropertybrowser/qttreepropertybrowser.h"
#include "qtpropertybrowser/qtvariantproperty.h"

#include <QMetaObject>
#include <QMetaProperty>

#include <QMetaProperty>
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(LogPropEditor2)
Q_LOGGING_CATEGORY(LogPropEditor2, "leda.schematics.propeditor")

using namespace SymbolEditor;

ObjectPropertyEditor::ObjectPropertyEditor(QWidget *parent):
    QtTreePropertyBrowser(parent),
    m_object(nullptr), m_populatingBrowser(false)
{
    setHeaderVisible(false);
    setAlternatingRowColors(true);
    setPropertiesWithoutValueMarked(true);
    setRootIsDecorated(false);

    m_manager = new QtVariantPropertyManager(this);
    m_factory = new QtVariantEditorFactory(this);
    setFactoryForManager(m_manager, m_factory);
    connect(m_manager, &QtVariantPropertyManager::valueChanged,
            this, &ObjectPropertyEditor::setObjectPropertyValue);

    m_penManager = new PenPropertyManager(this);
    m_penFactory = new PenEditorFactory(this);
    setFactoryForManager(m_penManager, m_penFactory);
    setFactoryForManager(m_penManager->subEnumManager(), m_penFactory->subEnumEditorFactory());
    setFactoryForManager(m_penManager->subColorManager(), m_penFactory->subColorEditorFactory());
    setFactoryForManager(m_penManager->subColorManager()->subIntPropertyManager(),
                         new QtSpinBoxFactory(this));
    connect(m_penManager, &PenPropertyManager::valueChanged,
            this, &ObjectPropertyEditor::setObjectPropertyValue);

}

ObjectPropertyEditor::~ObjectPropertyEditor()
{

}

void ObjectPropertyEditor::setObjects(QList<QObject *> objects)
{
    m_penManager->clear();
    m_manager->clear();
    clear();

    m_object = objects.isEmpty() ? nullptr : objects.first();
    if (m_object == nullptr)
    {
        return;
    }

    m_populatingBrowser = true;
    populateBrowser(m_object, m_object->metaObject());
    m_populatingBrowser = false;
}

void ObjectPropertyEditor::populateBrowser(QObject *object, const QMetaObject *metaObject)
{
    if (!object || !metaObject)
    {
        return;
    }

    populateBrowser(object, metaObject->superClass());
    QtProperty *parentProperty = m_manager->addProperty(QtVariantPropertyManager::groupTypeId(),
                                                        metaObject->className());
    for (int i = 0; i < metaObject->propertyCount(); ++i)
    {
        QMetaProperty metaProperty = metaObject->property(i);
        if (metaProperty.enclosingMetaObject() != metaObject)
        {
            continue; // ?!?
        }
        if (!metaProperty.isReadable())
        {
            continue;
        }
        if (!metaProperty.hasNotifySignal())
        {
            continue;
        }
        QtProperty *property;
        if (metaProperty.isEnumType())
        {
            if (metaProperty.isFlagType())
            {
                property = m_manager->addProperty(QtVariantPropertyManager::flagTypeId());
            }
            else
            {
                property = m_manager->addProperty(QtVariantPropertyManager::enumTypeId());
                QMetaEnum metaEnum = metaProperty.enumerator();
                QStringList enumNames;
                int enumValue = metaProperty.read(object).toInt();
                int enumIndex = -1;
                for (int i = 0; i < metaEnum.keyCount(); i++)
                {
                    if (enumValue == metaEnum.value(i))
                    {
                        enumIndex = i;
                    }
                    enumNames.append(QLatin1String(metaEnum.key(i)));
                }
                m_manager->setAttribute(property, QLatin1String("enumNames"), enumNames);
                if (enumIndex == -1)
                {
                    qCWarning(LogPropEditor2) << QString("Enum index/value not found for value of %1").arg(
                                                  property->propertyName());
                }
                else
                {
                    m_manager->setValue(property, enumIndex);
                }
            }
        }
        else if (metaProperty.type() == QVariant::Pen)
        {
            property = m_penManager->addProperty("Outline");
            m_penManager->setValue(property, metaProperty.read(object).value<QPen>());
        }
        else
        {
            property = m_manager->addProperty(metaProperty.type(),
                                              metaProperty.name());
            if (!property)
            {
                qCWarning(LogPropEditor2) << QString("Cannot add property %1::%2 of type %3")
                                          .arg(metaObject->className())
                                          .arg(metaProperty.name())
                                          .arg(metaProperty.typeName());
                continue;
            }
            m_manager->setValue(property, object->property(metaProperty.name()));
        }
        property->setEnabled(metaObject->property(i).isWritable());
        m_propertyToMetaPropertyIndex[property] = metaProperty.propertyIndex();
        parentProperty->addSubProperty(property);
    }
    QtBrowserItem *item = addProperty(parentProperty);
    for (QtBrowserItem *childItem : item->children())
    {
        setExpanded(childItem, false);
    }
}

void ObjectPropertyEditor::setObjectPropertyValue(QtProperty *property, const QVariant &value)
{
    if (m_populatingBrowser)
    {
        return;
    }

    if (!m_propertyToMetaPropertyIndex.contains(property))
    {
        qCWarning(LogPropEditor2) << "Unknown property";
        return;
    }
    int metaPropertyIndex = m_propertyToMetaPropertyIndex[property];
    QMetaProperty metaProperty = m_object->metaObject()->property(metaPropertyIndex);
    bool ok = metaProperty.write(m_object, value);
    if (!ok)
    {
        qCWarning(LogPropEditor2) << QString("Failed to set property %1::%2 (%3)")
                                  .arg(m_object->metaObject()->className())
                                  .arg(property->propertyName())
                                  .arg(value.typeName());
    }
}

void ObjectPropertyEditor::setBrowserPropertyValue(QtProperty *property, const QVariant &value)
{
    Q_UNUSED(property);
    Q_UNUSED(value);
}
