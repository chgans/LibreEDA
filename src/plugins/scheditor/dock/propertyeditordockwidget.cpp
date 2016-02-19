#include "propertyeditordockwidget.h"
#include "propertyeditor/penpropertymanager.h"
#include "propertyeditor/peneditorfactory.h"

#include "qtpropertybrowser/qttreepropertybrowser.h"
#include "qtpropertybrowser/qtvariantproperty.h"
#include "item/schitem.h"

#include <QMetaProperty>
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(LogPropEditor)
Q_LOGGING_CATEGORY(LogPropEditor, "leda.schematics.propeditor")

PropertyEditorDockWidget::PropertyEditorDockWidget():
    QDockWidget("Property editor"),
    m_object(nullptr), m_populatingBrowser(false)
{
    m_browser = new QtTreePropertyBrowser;
    m_browser->setHeaderVisible(false);
    m_browser->setAlternatingRowColors(true);
    m_browser->setPropertiesWithoutValueMarked(true);
    m_browser->setRootIsDecorated(false);
    m_manager = new QtVariantPropertyManager(this);
    m_factory = new QtVariantEditorFactory(this);
    m_browser->setFactoryForManager(m_manager, m_factory);
    connect(m_manager, &QtVariantPropertyManager::valueChanged,
            this, &PropertyEditorDockWidget::setObjectPropertyValue);
    m_penManager = new PenPropertyManager(this);
    m_penFactory = new PenEditorFactory(this);
    m_browser->setFactoryForManager(m_penManager, m_penFactory);
    m_browser->setFactoryForManager(m_penManager->subEnumManager(), m_penFactory->subEnumEditorFactory());
    m_browser->setFactoryForManager(m_penManager->subColorManager(), m_penFactory->subColorEditorFactory());
    m_browser->setFactoryForManager(m_penManager->subColorManager()->subIntPropertyManager(), new QtSpinBoxFactory(this));
    connect(m_penManager, &PenPropertyManager::valueChanged,
            this, &PropertyEditorDockWidget::setObjectPropertyValue);

    setWidget(m_browser);
}

void PropertyEditorDockWidget::setObject(QObject *object)
{
    m_penManager->clear();
    m_manager->clear();
    m_browser->clear();

    m_object = object;
    m_item = nullptr;
    if (m_object == nullptr)
        return;

    m_populatingBrowser = true;
    populateBrowser(m_object, m_object->metaObject());
    m_populatingBrowser = false;
}

void PropertyEditorDockWidget::setItem(SchItem *item)
{

    m_manager->clear();
    m_browser->clear();

    if (m_item != nullptr)
        m_item->disconnect(this);

    m_item = item;
    m_object = nullptr;
    if (m_item == nullptr)
        return;

    m_populatingBrowser = true;
    populateBrowser(m_item);
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
            continue; // ?!?
        }
        if (!metaProperty.isReadable()) {
            continue;
        }
        QtProperty *property;
        if (metaProperty.isEnumType()) {
            if (metaProperty.isFlagType()) {
                property = m_manager->addProperty(QtVariantPropertyManager::flagTypeId());
            }
            else {
                property = m_manager->addProperty(QtVariantPropertyManager::enumTypeId());
                QMetaEnum metaEnum = metaProperty.enumerator();
                QStringList enumNames;
                int enumValue = metaProperty.read(object).toInt();
                int enumIndex;
                for (int i = 0; i < metaEnum.keyCount(); i++) {
                    if (enumValue == metaEnum.value(i))
                        enumIndex = i;
                    enumNames.append(QLatin1String(metaEnum.key(i)));
                }
                m_manager->setAttribute(property, QLatin1String("enumNames"), enumNames);
                m_manager->setValue(property, enumIndex);
            }
        }
        else if (metaProperty.type() == QVariant::Pen) {
            property = m_penManager->addProperty("Outline");
            m_penManager->setValue(property, metaProperty.read(object).value<QPen>());
        }
        else {
            property = m_manager->addProperty(metaProperty.type(),
                                              metaProperty.name());
            if (!property) {
                qCWarning(LogPropEditor) << QString("Cannot add property %1::%2 of type %3")
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
    QtBrowserItem *item = m_browser->addProperty(parentProperty);
    foreach (QtBrowserItem *childItem, item->children()) {
        m_browser->setExpanded(childItem, false);
    }
}

// TODO: don't use QtVariantProperty/QtVariantPropertyManager/QtVariantEditorFactory
// TODO: Use QtProperty/Qt<Type>PropertyManager/Qt<Type>EditorFactory
// TODO: Add prefix/suffix (typ. to display unit)
// TODO: Add unit selection
// TODO: For colors, use combo box with palette's color, plus "..." for custom colors (instead of RGBA sub-properties)
void PropertyEditorDockWidget::populateBrowser(SchItem *item)
{
    QtVariantProperty *groupProperty;
    QtVariantProperty *property;

    qCInfo(LogPropEditor()) << item << m_item;

    groupProperty = m_manager->addProperty(QtVariantPropertyManager::groupTypeId(), "Attributes");
    property = m_manager->addProperty(QVariant::Bool, "Locked");
    m_manager->setValue(property, !m_item->isEnabled());
    groupProperty->addSubProperty(property);
    m_browser->addProperty(groupProperty);
    // TBD: visible (dangerous)

    groupProperty = m_manager->addProperty(QtVariantPropertyManager::groupTypeId(), "Geometry");

    QtVariantProperty *xProperty = m_manager->addProperty(QVariant::Double, "X");
    xProperty->setAttribute("decimals", 2);
    xProperty->setAttribute("singleStep", 1);
    xProperty->setModified(true);
    m_manager->setValue(xProperty, m_item->x());
    groupProperty->addSubProperty(xProperty);
    connect(m_item, &SchItem::xChanged,
            this, [this, xProperty](){
        m_manager->setValue(xProperty, m_item->x());
    });

    QtVariantProperty *yProperty = m_manager->addProperty(QVariant::Double, "Y");
    yProperty->setAttribute("decimals", 2);
    yProperty->setAttribute("singleStep", 1);
    m_manager->setValue(yProperty, m_item->y());
    groupProperty->addSubProperty(yProperty);
    connect(m_item, &SchItem::yChanged,
            this, [this, yProperty](){
        m_manager->setValue(yProperty, m_item->y());
    });

    property = m_manager->addProperty(QVariant::Double, "Z Value");
    property->setAttribute("decimals", 2);
    property->setAttribute("singleStep", 1);
    m_manager->setValue(property, m_item->zValue());
    groupProperty->addSubProperty(property);

    property = m_manager->addProperty(QVariant::Double, "Opacity");
    property->setAttribute("minimum", 0.0);
    property->setAttribute("maximum", 1.0);
    property->setAttribute("decimals", 2);
    property->setAttribute("singleStep", 0.1);
    m_manager->setValue(property, m_item->opacity());
    groupProperty->addSubProperty(property);

    property = m_manager->addProperty(QVariant::Double, "Rotation");
    property->setAttribute("minimum", 0.0);
    property->setAttribute("maximum", 359.99);
    property->setAttribute("singleStep", 10);
    property->setAttribute("decimals", 2);
    m_manager->setValue(property, m_item->rotation());
    groupProperty->addSubProperty(property);

    property = m_manager->addProperty(QVariant::Bool, "X Mirrored");
    property->setValue(m_item->isXMirrored());
    groupProperty->addSubProperty(property);

    property = m_manager->addProperty(QVariant::Bool, "Y Mirrored");
    property->setValue(m_item->isYMirrored());
    groupProperty->addSubProperty(property);
    m_browser->addProperty(groupProperty);
    // TODO: item specifics

    // TODO: outline for shape vs line (for segment, polyline, bezier)
    // TODO: "simple" line can have start/end arrows
    groupProperty = m_manager->addProperty(QtVariantPropertyManager::groupTypeId(), "Line");

    property = m_manager->addProperty(QVariant::Double, "Width");
    property->setAttribute("minimum", 0.0);
    property->setAttribute("decimals", 2);
    property->setAttribute("singleStep", 0.1);
    m_manager->setValue(property, m_item->pen().widthF());
    groupProperty->addSubProperty(property);

    property = m_manager->addProperty(QVariant::Color, "Color");
    m_manager->setValue(property, m_item->pen().color());
    groupProperty->addSubProperty(property);
    property = m_manager->addProperty(QtVariantPropertyManager::enumTypeId(), "Style");
    property->setAttribute("enumNames", QStringList() << "None" << "Solid" << "Dash" << "Dash Dot" << "Dash Dot Dot");
    //property->setAttribute("enumIcons", );
    m_manager->setValue(property, int(m_item->pen().style())); // FIXME: value is enum index
    groupProperty->addSubProperty(property);

    property = m_manager->addProperty(QtVariantPropertyManager::enumTypeId(), "Cap style");
    property->setAttribute("enumNames", QStringList() << "Flat" << "Square" << "Round");
    //property->setAttribute("enumIcons", );
    m_manager->setValue(property, int(m_item->pen().capStyle())); // FIXME: value is enum index
    groupProperty->addSubProperty(property);

    property = m_manager->addProperty(QtVariantPropertyManager::enumTypeId(), "Join style");
    property->setAttribute("enumNames", QStringList() << "Miter" << "Bevel" << "Round");
    //property->setAttribute("enumIcons", );
    m_manager->setValue(property, int(m_item->pen().joinStyle())); // FIXME: value is enum index
    groupProperty->addSubProperty(property);
    m_browser->addProperty(groupProperty);

    groupProperty = m_manager->addProperty(QtVariantPropertyManager::groupTypeId(), "Fill");

    property = m_manager->addProperty(QVariant::Color, "Color "); // FIXME: " " for pen/brush
    m_manager->setValue(property, m_item->brush().color());
    groupProperty->addSubProperty(property);
    m_browser->addProperty(groupProperty);
}

void PropertyEditorDockWidget::setObjectPropertyValue(QtProperty *property, const QVariant &value)
{
    if (m_populatingBrowser)
        return;

    if (m_object != nullptr) {
        if (!m_propertyToMetaPropertyIndex.contains(property)) {
            qCWarning(LogPropEditor) << "Unknown property";
            return;
        }
        int metaPropertyIndex = m_propertyToMetaPropertyIndex[property];
        QMetaProperty metaProperty = m_object->metaObject()->property(metaPropertyIndex);
        bool ok = metaProperty.write(m_object, value);
        if (!ok) {
            qCWarning(LogPropEditor) << QString("Failed to set property %1::%2 (%3)")
                                        .arg(m_object->metaObject()->className())
                                        .arg(property->propertyName())
                                        .arg(value.typeName());
        }
    }
    if (m_item != nullptr) {
        if (property->propertyName() == "Rotation") {
            m_item->setRotation(value.toReal());
        }
        else if (property->propertyName() == "Z Value") {
            m_item->setZValue(value.toReal());
        }
        else if (property->propertyName() == "Y") {
            m_item->setY(value.toReal());
        }
        else if (property->propertyName() == "X") {
            m_item->setX(value.toReal());
        }
        else if (property->propertyName() == "Width") {
            QPen pen = m_item->pen();
            pen.setWidthF(value.toDouble());
            m_item->setPen(pen);
        }
        else if (property->propertyName() == "Style") {
            QPen pen = m_item->pen();
            pen.setStyle(Qt::PenStyle(value.toInt()));
            m_item->setPen(pen);
        }
        else if (property->propertyName() == "Cap style") {
            QPen pen = m_item->pen();
            pen.setCapStyle(Qt::PenCapStyle(value.toInt()));
            m_item->setPen(pen);
        }
        else if (property->propertyName() == "Join style") {
            QPen pen = m_item->pen();
            pen.setJoinStyle(Qt::PenJoinStyle(value.toInt()));
            m_item->setPen(pen);
        }
        else if (property->propertyName() == "Color") {
            QPen pen = m_item->pen();
            pen.setColor(value.value<QColor>());
            m_item->setPen(pen);
        }
        else if (property->propertyName() == "Color ") { // FIXME: extra " "
            QBrush brush = m_item->brush();
            brush.setColor(value.value<QColor>());
            m_item->setBrush(brush);
        }
        else if (property->propertyName() == "Y Mirrored") {
            m_item->setYMirrored(value.toBool());
        }
        else if (property->propertyName() == "X Mirrored") {
            m_item->setXMirrored(value.toBool());
        }
        else if (property->propertyName() == "Locked") {
            m_item->setEnabled(!value.toBool());
        }
        else if (property->propertyName() == "Opacity") {
            m_item->setOpacity(value.toReal());
        }
    }
}

void PropertyEditorDockWidget::setBrowserPropertyValue(QtProperty *property, const QVariant &value)
{

}

