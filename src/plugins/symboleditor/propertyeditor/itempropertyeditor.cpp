// See https://github.com/qtproject/qttools/blob/dev/src/designer/src/
// - lib/sdk/abstractpropertyeditor.h
// - lib/shared/qdesigner_propertyeditor_p.h
// - components/propertyeditor/propertyeditor.h

#include "itempropertyeditor.h"
#include "propertybrowser.h"
#include "propertymanager.h"

#include "command/setpropertycommand.h"

#include <QVBoxLayout>

namespace SymbolEditor
{

    ItemPropertyEditor::ItemPropertyEditor(QWidget *parent):
        QWidget(parent), m_item(nullptr)
    {
        setLayout(new QVBoxLayout);

        // TODO: setBrowser();
        m_browser = new TreePropertyBrowser();
        layout()->addWidget(m_browser);
        layout()->setMargin(0);

        // TODO: setManager();
        m_manager = new PropertyManager(this);
        m_manager->setBrowserFactories(m_browser);

        connect(m_manager, &PropertyManager::propertyAdded,
                m_browser, &TreePropertyBrowser::addProperty);

        connect(m_manager, &PropertyManager::valueChanged,
                this, &ItemPropertyEditor::onValueChanged);

    }

    ItemPropertyEditor::~ItemPropertyEditor()
    {

    }

    const Document::Item *ItemPropertyEditor::item() const
    {
        return m_item;
    }

    void ItemPropertyEditor::setPalette(Palette palette)
    {
        m_palette = palette;
        m_manager->setPalette(m_palette);
    }

    Palette ItemPropertyEditor::palette() const
    {
        return m_palette;
    }

    void ItemPropertyEditor::onValueChanged(quint64 id, const QVariant &value)
    {
        if (m_updatingProperties)
        {
            return;
        }

        auto command = new SetPropertyCommand();
        command->setItemId(m_item->id());
        command->setPropertId(id);
        command->setPropertyValue(value);
        emit commandRequested(command);
    }

    void ItemPropertyEditor::setItem(const Document::Item *item)
    {
        if (m_item == item)
        {
            return;
        }

        clear();
        m_item = item;
        load();
    }

    void ItemPropertyEditor::updateProperty(quint64 id, const QVariant &value)
    {
        m_updatingProperties = true;
        m_manager->setValue(id, value);
        m_updatingProperties = false;
    }

    void ItemPropertyEditor::clear()
    {
        m_browser->clear();
        m_manager->clear();
    }

    void ItemPropertyEditor::load()
    {
        if (m_item == nullptr)
        {
            return;
        }

        m_updatingProperties = true;

        switch (m_item->type())
        {
            case xdl::symbol::Item::Circle:
                addCoordinate(xdl::symbol::Item::PositionProperty);
                addLength(xdl::symbol::Item::RadiusProperty);
                addLineStyle(xdl::symbol::Item::LineStyleProperty);
                addLineWidth(xdl::symbol::Item::LineWidthProperty);
                addColor(xdl::symbol::Item::LineColorProperty);
                addFillStyle(xdl::symbol::Item::FillStyleProperty);
                addColor(xdl::symbol::Item::FillColorProperty);
                break;
            case xdl::symbol::Item::CircularArc:
                addCoordinate(xdl::symbol::Item::PositionProperty);
                addLength(xdl::symbol::Item::RadiusProperty);
                addAngle(xdl::symbol::Item::StartAngleProperty);
                addAngle(xdl::symbol::Item::SpanAngleProperty);
                addLineStyle(xdl::symbol::Item::LineStyleProperty);
                addLineWidth(xdl::symbol::Item::LineWidthProperty);
                addColor(xdl::symbol::Item::LineColorProperty);
                addFillStyle(xdl::symbol::Item::FillStyleProperty);
                addColor(xdl::symbol::Item::FillColorProperty);
                break;
            case xdl::symbol::Item::Ellipse:
                addCoordinate(xdl::symbol::Item::PositionProperty);
                addLength(xdl::symbol::Item::XRadiusProperty);
                addLength(xdl::symbol::Item::YRadiusProperty);
                addLineStyle(xdl::symbol::Item::LineStyleProperty);
                addLineWidth(xdl::symbol::Item::LineWidthProperty);
                addColor(xdl::symbol::Item::LineColorProperty);
                addFillStyle(xdl::symbol::Item::FillStyleProperty);
                addColor(xdl::symbol::Item::FillColorProperty);
                break;
            case xdl::symbol::Item::EllipticalArc:
                addCoordinate(xdl::symbol::Item::PositionProperty);
                addLength(xdl::symbol::Item::XRadiusProperty);
                addLength(xdl::symbol::Item::YRadiusProperty);
                addAngle(xdl::symbol::Item::StartAngleProperty);
                addAngle(xdl::symbol::Item::SpanAngleProperty);
                addLineStyle(xdl::symbol::Item::LineStyleProperty);
                addLineWidth(xdl::symbol::Item::LineWidthProperty);
                addColor(xdl::symbol::Item::LineColorProperty);
                addFillStyle(xdl::symbol::Item::FillStyleProperty);
                addColor(xdl::symbol::Item::FillColorProperty);
                break;
            case xdl::symbol::Item::Polyline:
                addLineStyle(xdl::symbol::Item::LineStyleProperty);
                addLineWidth(xdl::symbol::Item::LineWidthProperty);
                addColor(xdl::symbol::Item::LineColorProperty);
                addFillStyle(xdl::symbol::Item::FillStyleProperty);
                addColor(xdl::symbol::Item::FillColorProperty);
                break;
            case xdl::symbol::Item::Polygon:
                addLineStyle(xdl::symbol::Item::LineStyleProperty);
                addLineWidth(xdl::symbol::Item::LineWidthProperty);
                addColor(xdl::symbol::Item::LineColorProperty);
                addFillStyle(xdl::symbol::Item::FillStyleProperty);
                addColor(xdl::symbol::Item::FillColorProperty);
                break;
            case xdl::symbol::Item::Label:
                break;
            case xdl::symbol::Item::Pin:
                break;
            case xdl::symbol::Item::Group:
                break;
            case xdl::symbol::Item::Rectangle:
                addCoordinate(xdl::symbol::Item::PositionProperty);
                addLength(xdl::symbol::Item::WidthProperty);
                addLength(xdl::symbol::Item::HeightProperty);
                addLineStyle(xdl::symbol::Item::LineStyleProperty);
                addLineWidth(xdl::symbol::Item::LineWidthProperty);
                addColor(xdl::symbol::Item::LineColorProperty);
                addFillStyle(xdl::symbol::Item::FillStyleProperty);
                addColor(xdl::symbol::Item::FillColorProperty);
                break;
        }
        addAngle(xdl::symbol::Item::RotationProperty);
        addBistate(xdl::symbol::Item::XMirroredProperty);
        addBistate(xdl::symbol::Item::YMirroredProperty);
        addBistate(xdl::symbol::Item::LockedProperty);
        addBistate(xdl::symbol::Item::VisibilityProperty);
        addPercentage(xdl::symbol::Item::OpacityProperty);

        m_updatingProperties = false;
    }

    void ItemPropertyEditor::addCoordinate(quint64 id)
    {
        m_manager->addCoordinate(id, m_item->friendlyPropertyName(id));
        m_manager->setValue(id, m_item->property(id));
    }

    void ItemPropertyEditor::addLength(quint64 id)
    {
        m_manager->addLength(id, m_item->friendlyPropertyName(id));
        m_manager->setValue(id, m_item->property(id));
    }

    void ItemPropertyEditor::addAngle(quint64 id)
    {
        m_manager->addAngle(id, m_item->friendlyPropertyName(id));
        m_manager->setValue(id, m_item->property(id));
    }

    void ItemPropertyEditor::addPercentage(quint64 id)
    {
        m_manager->addPercentage(id, m_item->friendlyPropertyName(id));
        m_manager->setValue(id, m_item->property(id));
    }

    void ItemPropertyEditor::addColor(quint64 id)
    {
        m_manager->addColor(id, m_item->friendlyPropertyName(id));
        m_manager->setValue(id, m_item->property(id));
    }

    void ItemPropertyEditor::addBistate(quint64 id)
    {
        m_manager->addBistate(id, m_item->friendlyPropertyName(id));
        m_manager->setValue(id, m_item->property(id));
    }

    void ItemPropertyEditor::addText(quint64 id)
    {
        m_manager->addText(id, m_item->friendlyPropertyName(id));
        m_manager->setValue(id, m_item->property(id));
    }

    void ItemPropertyEditor::addLineStyle(quint64 id)
    {
        m_manager->addLineStyle(id, m_item->friendlyPropertyName(id));
        m_manager->setValue(id, m_item->property(id));
    }

    void ItemPropertyEditor::addLineWidth(quint64 id)
    {
        m_manager->addLineWidth(id, m_item->friendlyPropertyName(id));
        m_manager->setValue(id, m_item->property(id));
    }

    void ItemPropertyEditor::addFillStyle(quint64 id)
    {
        m_manager->addFillStyle(id, m_item->friendlyPropertyName(id));
        m_manager->setValue(id, m_item->property(id));
    }

}
