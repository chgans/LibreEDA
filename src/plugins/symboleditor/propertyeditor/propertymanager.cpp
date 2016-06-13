#include "propertymanager.h"

#include "xdl/symbol.h"
#include "view/palette.h"

#include <qtpropertybrowser/qtpropertybrowser.h>
#include <qtpropertybrowser/qteditorfactory.h>
#include <qtpropertybrowser/qtpropertymanager.h>

#include <QVariant>

namespace SymbolEditor
{

    PropertyManager::PropertyManager(QObject *parent):
        QObject(parent)
    {
        m_enumManager = new QtEnumPropertyManager(this);
        connect(m_enumManager, &QtEnumPropertyManager::valueChanged,
                this, &PropertyManager::onEnumValueChanged);

        m_pointManager = new QtPointFPropertyManager(this);
        connect(m_pointManager, &QtPointFPropertyManager::valueChanged,
                this, &PropertyManager::onPointValueChanged);

        m_realManager = new QtDoublePropertyManager(this);
        connect(m_realManager, &QtDoublePropertyManager::valueChanged,
                this, &PropertyManager::onRealValueChanged);

        m_intManager = new QtIntPropertyManager(this);
        connect(m_intManager, &QtIntPropertyManager::valueChanged,
                this, &PropertyManager::onIntValueChanged);

        m_boolManager = new QtBoolPropertyManager(this);
        connect(m_boolManager, &QtBoolPropertyManager::valueChanged,
                this, &PropertyManager::onBoolValueChanged);

        m_stringManager = new QtStringPropertyManager(this);
        connect(m_stringManager, &QtStringPropertyManager::valueChanged,
                this, &PropertyManager::onStringValueChanged);
    }

    PropertyManager::~PropertyManager()
    {

    }

    void PropertyManager::setBrowserFactories(QtAbstractPropertyBrowser *browser)
    {
        browser->setFactoryForManager(m_enumManager, new QtEnumEditorFactory);
        browser->setFactoryForManager(m_pointManager->subDoublePropertyManager(), new QtDoubleSpinBoxFactory);
        browser->setFactoryForManager(m_realManager, new QtDoubleSpinBoxFactory);
        browser->setFactoryForManager(m_intManager, new QtSpinBoxFactory);
        browser->setFactoryForManager(m_boolManager, new QtCheckBoxFactory);
        browser->setFactoryForManager(m_stringManager, new QtLineEditFactory);
    }

    void PropertyManager::addCoordinate(quint64 id, const QString &name)
    {
        auto manager = m_pointManager;
        auto property = manager->addProperty(name);
        addProperty(id, property, manager);
    }

    void PropertyManager::addLength(quint64 id, const QString &name)
    {
        auto manager = m_realManager;
        auto property = manager->addProperty(name);
        manager->setMinimum(property, 0.0);
        manager->setSingleStep(property, 1.0);
        manager->setDecimals(property, 1);
        addProperty(id, property, manager);
    }

    void PropertyManager::addAngle(quint64 id, const QString &name)
    {
        auto manager = m_realManager;
        auto property = manager->addProperty(name);
        manager->setMinimum(property, 0);
        manager->setSingleStep(property, 1);
        manager->setMaximum(property, 360);
        addProperty(id, property, manager);
    }

    void PropertyManager::addPercentage(quint64 id, const QString &name)
    {
        auto manager = m_intManager;
        auto property = manager->addProperty(name);
        manager->setMinimum(property, 0);
        manager->setSingleStep(property, 1);
        manager->setMaximum(property, 100);
        addProperty(id, property, manager);
    }

    static QIcon colorIcon(const QColor &color)
    {
        QPixmap pix(16, 16);
        pix.fill(color);
        return QIcon(pix);
    }

    void PropertyManager::addColor(quint64 id, const QString &name)
    {
        auto manager = m_enumManager;
        auto property = manager->addProperty(name);


        manager->setEnumNames(property, m_colorNames.values());
        manager->setEnumIcons(property, m_colorIcons);
        addProperty(id, property, manager);
    }

    void PropertyManager::addBistate(quint64 id, const QString &name)
    {
        auto manager = m_boolManager;
        auto property = manager->addProperty(name);
        addProperty(id, property, manager);
    }

    void PropertyManager::addText(quint64 id, const QString &name)
    {
        auto manager = m_stringManager;
        auto property = manager->addProperty(name);
        addProperty(id, property, manager);
    }

    void PropertyManager::addLineStyle(quint64 id, const QString &name)
    {
        auto manager = m_enumManager;
        auto property = manager->addProperty(name);

        QMap<int, QString> enumNames;
        enumNames.insert(xdl::symbol::NoLine, "None");
        enumNames.insert(xdl::symbol::SolidLine, "Solid");
        enumNames.insert(xdl::symbol::DashLine, "Dash");
        enumNames.insert(xdl::symbol::DotLine, "Dot");
        enumNames.insert(xdl::symbol::DashDotLine, "Dash-dot");
        enumNames.insert(xdl::symbol::DashDotDotLine, "Dash-dot-dot");
        manager->setEnumNames(property, enumNames.values());
        addProperty(id, property, manager);
    }

    void PropertyManager::addLineWidth(quint64 id, const QString &name)
    {
        auto manager = m_enumManager;
        auto property = manager->addProperty(name);

        QMap<int, QString> enumNames;
        enumNames.insert(xdl::symbol::ThinestLine, "Thinest");
        enumNames.insert(xdl::symbol::ThinerLine, "Thiner");
        enumNames.insert(xdl::symbol::ThinLine, "Thin");
        enumNames.insert(xdl::symbol::SlightlyThinLine, "Slightly thin");
        enumNames.insert(xdl::symbol::MediumLine, "Medium");
        enumNames.insert(xdl::symbol::SlightlyThickLine, "Slightly thick");
        enumNames.insert(xdl::symbol::ThickLine, "Thick");
        enumNames.insert(xdl::symbol::ThickerLine, "Thicker");
        enumNames.insert(xdl::symbol::ThickestLine, "Thickest");
        manager->setEnumNames(property, enumNames.values());
        addProperty(id, property, manager);
    }

    void PropertyManager::addFillStyle(quint64 id, const QString &name)
    {
        auto manager = m_enumManager;
        auto property = manager->addProperty(name);

        QMap<int, QString> enumNames;
        enumNames.insert(xdl::symbol::NoLine, "None");
        enumNames.insert(xdl::symbol::SolidLine, "Solid");
        manager->setEnumNames(property, enumNames.values());
        addProperty(id, property, manager);
    }

    void PropertyManager::clear()
    {
        m_propertyToIdMap.clear();
        m_idToPropertyMap.clear();
        m_idToManagerMap.clear();
        m_colorProperties.clear();

        m_enumManager->clear();
        m_pointManager->clear();
        m_realManager->clear();
        m_intManager->clear();
        m_boolManager->clear();
        m_stringManager->clear();
    }

    QVariant PropertyManager::value(quint64 id) const
    {
        auto manager = m_idToManagerMap.value(id);
        auto property = m_idToPropertyMap.value(id);

        if (manager == m_enumManager)
        {
            return m_enumManager->value(property);
        }
        else if (manager == m_pointManager)
        {
            return m_pointManager->value(property);
        }
        else if (manager == m_realManager)
        {
            return m_realManager->value(property);
        }
        else if (manager == m_intManager)
        {
            return m_intManager->value(property);
        }
        else if (manager == m_boolManager)
        {
            return m_boolManager->value(property);
        }
        else if (manager == m_stringManager)
        {
            return m_stringManager->value(property);
        }

        return QVariant();
    }

    void PropertyManager::setValue(quint64 id, const QVariant &value)
    {
        auto manager = m_idToManagerMap.value(id);
        auto property = m_idToPropertyMap.value(id);

        if (manager == m_enumManager)
        {
            m_enumManager->setValue(property, value.toInt());
        }
        else if (manager == m_pointManager)
        {
            m_pointManager->setValue(property, value.toPointF());
        }
        else if (manager == m_realManager)
        {
            m_realManager->setValue(property, value.toReal());
        }
        else if (manager == m_intManager)
        {
            m_intManager->setValue(property, value.toInt());
        }
        else if (manager == m_boolManager)
        {
            m_boolManager->setValue(property, value.toBool());
        }
        else if (manager == m_stringManager)
        {
            m_stringManager->setValue(property, value.toString());
        }
    }

    QString PropertyManager::toolTip(quint64 id) const
    {
        return m_idToPropertyMap.value(id)->toolTip();
    }

    void PropertyManager::setToolTip(quint64 id, const QString &toolTip)
    {
        m_idToPropertyMap.value(id)->setToolTip(toolTip);
    }

    QString PropertyManager::statusTip(quint64 id) const
    {
        return m_idToPropertyMap.value(id)->statusTip();
    }

    void PropertyManager::setStatusTip(quint64 id, const QString &statusTip)
    {
        m_idToPropertyMap.value(id)->setStatusTip(statusTip);
    }

    QString PropertyManager::whatsThis(quint64 id) const
    {
        return m_idToPropertyMap.value(id)->whatsThis();
    }

    void PropertyManager::setWhatsThis(quint64 id, const QString &whatsThis)
    {
        m_idToPropertyMap.value(id)->setWhatsThis(whatsThis);
    }

    QString PropertyManager::name(quint64 id) const
    {
        return m_idToPropertyMap.value(id)->propertyName();
    }

    void PropertyManager::setName(quint64 id, const QString &name)
    {
        m_idToPropertyMap.value(id)->setPropertyName(name);
    }

    bool PropertyManager::isEnabled(quint64 id) const
    {
        return m_idToPropertyMap.value(id)->isEnabled();
    }

    void PropertyManager::setEnabled(quint64 id, bool enable)
    {
        m_idToPropertyMap.value(id)->setEnabled(enable);
    }

    bool PropertyManager::isModified(quint64 id) const
    {
        return m_idToPropertyMap.value(id)->isModified();
    }

    void PropertyManager::setModified(quint64 id, bool modified)
    {
        m_idToPropertyMap.value(id)->setModified(modified);
    }

    void PropertyManager::setPalette(Palette palette)
    {
        m_palette = palette;
        updateColors();
    }

    Palette PropertyManager::palette() const
    {
        return m_palette;
    }

    void PropertyManager::onEnumValueChanged(QtProperty *property, int value)
    {
        emit valueChanged(m_propertyToIdMap.value(property), QVariant(value));
    }

    void PropertyManager::onPointValueChanged(QtProperty *property, const QPointF &value)
    {
        emit valueChanged(m_propertyToIdMap.value(property), QVariant(value));
    }

    void PropertyManager::onRealValueChanged(QtProperty *property, qreal value)
    {
        emit valueChanged(m_propertyToIdMap.value(property), QVariant(value));
    }

    void PropertyManager::onIntValueChanged(QtProperty *property, int value)
    {
        emit valueChanged(m_propertyToIdMap.value(property), QVariant(value));
    }

    void PropertyManager::onBoolValueChanged(QtProperty *property, bool value)
    {
        emit valueChanged(m_propertyToIdMap.value(property), QVariant(value));
    }

    void PropertyManager::onStringValueChanged(QtProperty *property, const QString &value)
    {
        emit valueChanged(m_propertyToIdMap.value(property), QVariant(value));
    }

    void PropertyManager::updateColors()
    {
        m_colorNames.clear();
        m_colorNames.insert(xdl::symbol::Background, "Background");
        m_colorNames.insert(xdl::symbol::BackgroundHighlight, "Background highlight");
        m_colorNames.insert(xdl::symbol::PrimaryContent, "Primary content");
        m_colorNames.insert(xdl::symbol::SecondaryContent, "Secondary content");
        m_colorNames.insert(xdl::symbol::EmphasisedContent, "Emphasised content");
        m_colorNames.insert(xdl::symbol::Yellow, "Yellow");
        m_colorNames.insert(xdl::symbol::Orange, "Orange");
        m_colorNames.insert(xdl::symbol::Red, "Red");
        m_colorNames.insert(xdl::symbol::Magenta, "Magenta");
        m_colorNames.insert(xdl::symbol::Violet, "Violet");
        m_colorNames.insert(xdl::symbol::Blue, "Blue");
        m_colorNames.insert(xdl::symbol::Cyan, "Cyan");
        m_colorNames.insert(xdl::symbol::Green, "Green");
        m_colorIcons.clear();
        m_colorIcons.insert(xdl::symbol::Background, colorIcon(m_palette.background()));
        m_colorIcons.insert(xdl::symbol::BackgroundHighlight, colorIcon(m_palette.backgroundHighlight()));
        m_colorIcons.insert(xdl::symbol::PrimaryContent, colorIcon(m_palette.primaryContent()));
        m_colorIcons.insert(xdl::symbol::SecondaryContent, colorIcon(m_palette.secondaryContent()));
        m_colorIcons.insert(xdl::symbol::EmphasisedContent, colorIcon(m_palette.emphasisedContent()));
        m_colorIcons.insert(xdl::symbol::Yellow, colorIcon(m_palette.yellow()));
        m_colorIcons.insert(xdl::symbol::Orange, colorIcon(m_palette.orange()));
        m_colorIcons.insert(xdl::symbol::Red, colorIcon(m_palette.red()));
        m_colorIcons.insert(xdl::symbol::Magenta, colorIcon(m_palette.magenta()));
        m_colorIcons.insert(xdl::symbol::Violet, colorIcon(m_palette.violet()));
        m_colorIcons.insert(xdl::symbol::Blue, colorIcon(m_palette.blue()));
        m_colorIcons.insert(xdl::symbol::Cyan, colorIcon(m_palette.cyan()));
        m_colorIcons.insert(xdl::symbol::Green, colorIcon(m_palette.green()));
        for (auto property: m_colorProperties)
        {
            m_enumManager->setEnumNames(property, m_colorNames.values());
            m_enumManager->setEnumIcons(property, m_colorIcons);
        }
    }

    void PropertyManager::addProperty(quint64 id, QtProperty *property, QtAbstractPropertyManager *manager)
    {
        m_propertyToIdMap.insert(property, id);
        m_idToPropertyMap.insert(id, property);
        m_idToManagerMap.insert(id, manager);

        emit propertyAdded(property);
    }

}
