#include "penpropertymanager.h"

QStringList penWidthEnumNames()
{
    return QStringList() << "Cosmetic" << "Thin" << "Medium" << "Thick";
}

int penWidthToEnumIndex(const QPen &pen)
{
    if (pen.widthF() > 1.0)
        return 3;
    if (pen.widthF() > 0.5)
        return 2;
    if (pen.widthF() > 0.0)
        return 1;
    return 0;
}

qreal enumIndexToPenWidth(int index)
{
    if (index == 0)
        return 0.0;
    if (index == 1)
        return 0.5;
    if (index == 2)
        return 1.0;
    if (index == 3)
        return 2.0;
    return 2;
}

QStringList penStyleEnumNames()
{
    return QStringList() << "Solid";
}

int penStyleToEnumIndex(const QPen &pen)
{
    return 0;
}

Qt::PenStyle enumIndexToPenStyle(int index)
{
    return Qt::SolidLine;
}

PenPropertyManager::PenPropertyManager(QObject *parent):
    QtAbstractPropertyManager(parent),
    m_enumManager(new QtEnumPropertyManager(this)),
    m_doubleManager(new QtDoublePropertyManager(this))
{
    connect(m_enumManager, &QtEnumPropertyManager::valueChanged,
            this, &PenPropertyManager::slotEnumChanged);
}

PenPropertyManager::~PenPropertyManager()
{
    clear();
}

QtEnumPropertyManager *PenPropertyManager::subEnumManager() const
{
    return m_enumManager;
}

QtDoublePropertyManager *PenPropertyManager::subDoubleManager() const
{
    return m_doubleManager;
}

QPen PenPropertyManager::value(const QtProperty *property) const
{
    QPen result;
    result.setWidthF(enumIndexToPenWidth(m_enumManager->value(m_penToWidthProperty[property])));
    result.setStyle(enumIndexToPenStyle(m_enumManager->value(m_penToStyleProperty[property])));
    return result;
}

void PenPropertyManager::setValue(QtProperty *property, const QPen &pen)
{
    m_enumManager->setValue(m_penToStyleProperty[property], penStyleToEnumIndex(pen));
    m_enumManager->setValue(m_penToWidthProperty[property], penWidthToEnumIndex(pen));

    emit propertyChanged(property);
    emit valueChanged(property, value(property));
}

QString PenPropertyManager::valueText(const QtProperty *property) const
{
    return QString(); //FIXME: [#rgb, ...]
}

void PenPropertyManager::initializeProperty(QtProperty *property)
{
    QtProperty *styleProperty = m_enumManager->addProperty("Style");
    m_enumManager->setEnumNames(styleProperty, penStyleEnumNames());
    m_styleToPenProperty[styleProperty] = property;
    m_penToStyleProperty[property] = styleProperty;
    property->addSubProperty(styleProperty);

    QtProperty *widthProperty = m_enumManager->addProperty("Width");
    m_enumManager->setEnumNames(widthProperty, penWidthEnumNames());
    m_widthToPenProperty[widthProperty] = property;
    m_penToWidthProperty[property] = widthProperty;
    property->addSubProperty(widthProperty);

    QtProperty *fakeProperty = m_doubleManager->addProperty("Fake");
    property->addSubProperty(fakeProperty);
}

void PenPropertyManager::uninitializeProperty(QtProperty *property)
{
    QtProperty *styleProperty = m_penToStyleProperty[property];
    if (styleProperty) {
        m_styleToPenProperty.remove(styleProperty);
        delete styleProperty;
    }
    m_penToStyleProperty.remove(property);

    QtProperty *widthProperty = m_penToWidthProperty[property];
    if (widthProperty) {
        m_widthToPenProperty.remove(widthProperty);
        delete widthProperty;
    }
    m_penToWidthProperty.remove(property);
}

void PenPropertyManager::slotEnumChanged(QtProperty *property, int val)
{
    Q_UNUSED(val)

    QtProperty *penProperty;
    if (m_penToStyleProperty.contains(property))
        penProperty = m_penToStyleProperty[property];
    else
        penProperty = m_penToWidthProperty[property];

    emit propertyChanged(penProperty);
    emit valueChanged(penProperty, value(penProperty));
}

