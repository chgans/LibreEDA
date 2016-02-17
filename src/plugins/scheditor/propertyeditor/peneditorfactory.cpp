#include "peneditorfactory.h"

PenEditorFactory::PenEditorFactory(QObject *parent):
    QtAbstractEditorFactory<PenPropertyManager>(parent)
{
    m_comboBoxFactory = new QtEnumEditorFactory(this);
    m_colorFactory = new QtColorEditorFactory(this);
}

PenEditorFactory::~PenEditorFactory()
{

}

QtEnumEditorFactory *PenEditorFactory::subEnumEditorFactory()
{
    return m_comboBoxFactory;
}

QtColorEditorFactory *PenEditorFactory::subColorEditorFactory()
{
    return m_colorFactory;
}

void PenEditorFactory::connectPropertyManager(PenPropertyManager *manager)
{
    m_comboBoxFactory->addPropertyManager(manager->subEnumManager());
    m_colorFactory->addPropertyManager(manager->subColorManager());
}

QWidget *PenEditorFactory::createEditor(PenPropertyManager *manager, QtProperty *property, QWidget *parent)
{
    QtAbstractEditorFactoryBase *factory = m_comboBoxFactory;
    return factory->createEditor(property, parent);
}

void PenEditorFactory::disconnectPropertyManager(PenPropertyManager *manager)
{
    m_colorFactory->removePropertyManager(manager->subColorManager());
    m_comboBoxFactory->removePropertyManager(manager->subEnumManager());
}
