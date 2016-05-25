#include "itempropertymanager.h"

using namespace SymbolEditor;

ItemPropertyManager::ItemPropertyManager(QObject *parent):
    QtAbstractPropertyManager(parent)
{
    m_realPropertyManager = new QtDoublePropertyManager(this);
    m_boolPropertyManager = new QtBoolPropertyManager(this);

    // opacity: real [0:0.1:1]
    auto opacityProperty = m_realPropertyManager->addProperty("Opacity");
    m_realPropertyManager->setMinimum(opacityProperty, 0.0);
    m_realPropertyManager->setMaximum(opacityProperty, 1.0);
    m_realPropertyManager->setSingleStep(opacityProperty, 0.1);

    // rotation: real [0:0.1:360]
    auto rotationProperty = m_realPropertyManager->addProperty("Rotation");
    m_realPropertyManager->setMinimum(rotationProperty, 0.0);
    m_realPropertyManager->setMaximum(rotationProperty, 360.0);
    m_realPropertyManager->setSingleStep(rotationProperty, 0.1);

    // zValue: real ?
    auto zValueProperty = m_realPropertyManager->addProperty("Z Value");
    m_realPropertyManager->setMinimum(zValueProperty, 0.0);
    //m_realPropertyManager->setMaximum(zValueProperty, 0.0);
    //m_realPropertyManager->setSingleStep(zValueProperty, 0.0);

    // locked: bool
    auto lockedProperty = m_boolPropertyManager->addProperty("Locked");

    // visible: bool
    auto visibleProperty = m_boolPropertyManager->addProperty("Visible");

    // X Mirrored: bool
    auto xMirroredProperty = m_boolPropertyManager->addProperty("X Mirrored");

    // Y Mirrored: bool
    auto yMirroreProperty = m_boolPropertyManager->addProperty("Y Mirrored");

    // TBD: Pen and brush (per item)
}

ItemPropertyManager::~ItemPropertyManager()
{

}
