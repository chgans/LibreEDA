#include "peneditorfactory.h"

PenEditorFactory::PenEditorFactory(QObject *parent):
    QtAbstractEditorFactory<PenPropertyManager>(parent)
{
    m_comboBoxFactory = new QtEnumEditorFactory(this);
    m_spinBoxFactory = new QtDoubleSpinBoxFactory(this);
}

PenEditorFactory::~PenEditorFactory()
{

}

void PenEditorFactory::connectPropertyManager(PenPropertyManager *manager)
{
    m_comboBoxFactory->addPropertyManager(manager->subEnumManager());
    m_spinBoxFactory->addPropertyManager(manager->subDoubleManager());
}

QWidget *PenEditorFactory::createEditor(PenPropertyManager *manager, QtProperty *property, QWidget *parent)
{
    QtAbstractEditorFactoryBase *factory = m_comboBoxFactory;
    return factory->createEditor(property, parent);
}

void PenEditorFactory::disconnectPropertyManager(PenPropertyManager *manager)
{
    m_spinBoxFactory->removePropertyManager(manager->subDoubleManager());
    m_comboBoxFactory->removePropertyManager(manager->subEnumManager());
}
