#include "propertyeditordockwidget.h"

#include "qtpropertybrowser/qttreepropertybrowser.h"
#include "qtpropertybrowser/qtvariantproperty.h"

#include <QMetaProperty>
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(LogPropEditor)
Q_LOGGING_CATEGORY(LogPropEditor, "leda.schematics.propeditor")

PropertyEditorDockWidget::PropertyEditorDockWidget():
    QDockWidget("Property editor"),
    m_object(nullptr), m_populatingBrowser(false)
{
    m_browser = new QtTreePropertyBrowser;
    m_manager = new QtVariantPropertyManager(this);
    m_factory = new QtVariantEditorFactory(this);
    m_browser->setFactoryForManager(m_manager, m_factory);
    connect(m_manager, &QtVariantPropertyManager::valueChanged,
            this, &PropertyEditorDockWidget::assignProperty);
    setWidget(m_browser);
}

void PropertyEditorDockWidget::setObject(QObject *object)
{
    m_manager->clear();
    m_browser->clear();

    m_object = object;
    if (m_object == nullptr)
        return;

    m_populatingBrowser = true;
    populateBrowser(m_object, m_object->metaObject());
    m_populatingBrowser = false;
}

void PropertyEditorDockWidget::populateBrowser(QObject *object, const QMetaObject *metaObject)
{
    if (!object || !metaObject)
        return;

    populateBrowser(object, metaObject->superClass());
    QtProperty *parentProperty = m_manager->addProperty(QtVariantPropertyManager::groupTypeId(),
                                                    metaObject->className());
    for (int i=0; i<metaObject->propertyCount(); ++i) {
        QMetaProperty metaProperty = metaObject->property(i);
        if (metaProperty.enclosingMetaObject() != metaObject) {
            continue;
        }
        QtProperty *property = m_manager->addProperty(metaProperty.type(),
                                                      metaProperty.name());
        if (!property) {
            qCWarning(LogPropEditor) << QString("Cannot add property %1::%2 of type %3")
                                          .arg(metaObject->className())
                                          .arg(metaProperty.name())
                                          .arg(metaProperty.typeName());
            continue;
        }
        m_manager->setValue(property, object->property(metaProperty.name()));
        property->setEnabled(metaObject->property(i).isWritable());
        parentProperty->addSubProperty(property);
    }
    QtBrowserItem *item = m_browser->addProperty(parentProperty);
    foreach (QtBrowserItem *childItem, item->children()) {
        m_browser->setExpanded(childItem, false);
    }
}

void PropertyEditorDockWidget::assignProperty(QtProperty *property, const QVariant &value)
{
    if (m_populatingBrowser)
        return;

    QByteArray name = property->propertyName().toLatin1();
    bool ok = m_object->setProperty(name.constData(), value);
    if (!ok) {
        qCWarning(LogPropEditor) << QString("Failed to set property %1::%2 (%3)")
                                    .arg(m_object->metaObject()->className())
                                    .arg(property->propertyName())
                                    .arg(value.typeName());
    }
}

