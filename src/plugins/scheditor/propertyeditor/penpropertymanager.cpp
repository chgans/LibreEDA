#include "penpropertymanager.h"

#include <QPixmap>
#include <QPainter>
#include <QIcon>

QIcon penWidthIcon(qreal width)
{
    QPixmap pixmap(16,16);
    QPainter painter;
    painter.begin(&pixmap);
    painter.fillRect(pixmap.rect(), Qt::white);
    painter.setPen(QPen(QBrush(Qt::black), width));
    painter.drawLine(2, 2, 14, 14);
    painter.end();
    return QIcon(pixmap);
}

QMap<int, QIcon> penWidthEnumIcons()
{
    QMap<int, QIcon> result;
    result[0] = penWidthIcon(0.0);
    result[1] = penWidthIcon(0.5);
    result[2] = penWidthIcon(1.0);
    result[3] = penWidthIcon(2.0);
    return result;
}

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

QIcon penStyleIcon(Qt::PenStyle style)
{
    QPixmap pixmap(16,16);
    QPainter painter;
    painter.begin(&pixmap);
    painter.fillRect(pixmap.rect(), Qt::white);
    painter.setPen(style);
    painter.drawLine(2, 2, 14, 14);
    painter.end();
    return QIcon(pixmap);
}

QMap<int, QIcon> penStyleEnumIcons()
{
    QMap<int, QIcon> result;
    result[0] = penStyleIcon(Qt::SolidLine);
    result[1] = penStyleIcon(Qt::DashLine);
    result[2] = penStyleIcon(Qt::DotLine);
    result[3] = penStyleIcon(Qt::DashDotLine);
    result[4] = penStyleIcon(Qt::DashDotDotLine);
    result[5] = penStyleIcon(Qt::NoPen);
    return result;
}

QStringList penStyleEnumNames()
{
    return QStringList() << "Solid" << "Dash" << "Dot" << "Dash Dot" << "Dash Dot Dot" << "None";
}

int penStyleToEnumIndex(const QPen &pen)
{
    switch (pen.style()) {
    case Qt::SolidLine: return 0;
    case Qt::DashLine: return 1;
    case Qt::DotLine: return 2;
    case Qt::DashDotLine: return 3;
    case Qt::DashDotDotLine: return 4;
    case Qt::NoPen: return 5;
    default:
        return 0;
    }
    return 0;
}

Qt::PenStyle enumIndexToPenStyle(int index)
{
    switch (index) {
    case 0: return Qt::SolidLine;
    case 1: return Qt::DashLine;
    case 2: return Qt::DotLine;
    case 3: return Qt::DashDotLine;
    case 4: return Qt::DashDotDotLine;
    case 5: return Qt::NoPen;
    default:
        return Qt::SolidLine;
    }
    return Qt::SolidLine;
}

PenPropertyManager::PenPropertyManager(QObject *parent):
    QtAbstractPropertyManager(parent),
    m_enumManager(new QtEnumPropertyManager(this)),
    m_colorManager(new QtColorPropertyManager(this))
{
    connect(m_enumManager, &QtEnumPropertyManager::valueChanged,
            this, &PenPropertyManager::slotEnumChanged);
    connect(m_colorManager, &QtColorPropertyManager::valueChanged,
            this, &PenPropertyManager::slotColorChanged);
}

PenPropertyManager::~PenPropertyManager()
{
    clear();
}

QtEnumPropertyManager *PenPropertyManager::subEnumManager() const
{
    return m_enumManager;
}

QtColorPropertyManager *PenPropertyManager::subColorManager() const
{
    return m_colorManager;
}

QPen PenPropertyManager::value(const QtProperty *property) const
{
    QPen result;
    result.setWidthF(enumIndexToPenWidth(m_enumManager->value(m_penToWidthProperty[property])));
    result.setStyle(enumIndexToPenStyle(m_enumManager->value(m_penToStyleProperty[property])));
    result.setColor(m_colorManager->value(m_penToColorProperty[property]));
    return result;
}

void PenPropertyManager::setValue(QtProperty *property, const QPen &pen)
{
    m_enumManager->setValue(m_penToStyleProperty[property], penStyleToEnumIndex(pen));
    m_enumManager->setValue(m_penToWidthProperty[property], penWidthToEnumIndex(pen));
    m_colorManager->setValue(m_penToColorProperty[property], pen.color());

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
    m_enumManager->setEnumIcons(styleProperty, penStyleEnumIcons());
    m_styleToPenProperty[styleProperty] = property;
    m_penToStyleProperty[property] = styleProperty;
    property->addSubProperty(styleProperty);

    QtProperty *widthProperty = m_enumManager->addProperty("Width");
    m_enumManager->setEnumNames(widthProperty, penWidthEnumNames());
    m_enumManager->setEnumIcons(widthProperty, penWidthEnumIcons());
    m_widthToPenProperty[widthProperty] = property;
    m_penToWidthProperty[property] = widthProperty;
    property->addSubProperty(widthProperty);

    QtProperty *colorProperty = m_colorManager->addProperty(("Color"));
    m_colorToPenProperty[colorProperty] = property;
    m_penToColorProperty[property] = colorProperty;
    property->addSubProperty(colorProperty);
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

    QtProperty *colorProperty = m_penToColorProperty[property];
    if (colorProperty) {
        m_colorToPenProperty.remove(colorProperty);
        delete colorProperty;
    }
    m_penToColorProperty.remove(property);
}

void PenPropertyManager::slotEnumChanged(QtProperty *property, int val)
{
    Q_UNUSED(val)

    QtProperty *penProperty;
    if (m_styleToPenProperty.contains(property))
        penProperty = m_styleToPenProperty[property];
    else if (m_widthToPenProperty.contains(property))
        penProperty = m_widthToPenProperty[property];
    else {
        qDebug() << "unknown property" << property;
        return;
    }

    emit propertyChanged(penProperty);
    emit valueChanged(penProperty, value(penProperty));
}

void PenPropertyManager::slotColorChanged(QtProperty *property, const QColor &val)
{
    Q_UNUSED(val)

    QtProperty *penProperty;
    if (m_colorToPenProperty.contains(property))
        penProperty = m_colorToPenProperty[property];
    else {
        qDebug() << "unknown property" << property;
        return;
    }

    emit propertyChanged(penProperty);
    emit valueChanged(penProperty, value(penProperty));
}

void PenPropertyManager::slotPropertyDestroyed(QtProperty *property)
{

}

