#ifndef PENEDITORFACTORY_H
#define PENEDITORFACTORY_H

#include "qtpropertybrowser/qteditorfactory.h"
#include "penpropertymanager.h"

class PenEditorFactory: public QtAbstractEditorFactory<PenPropertyManager>
{
    Q_OBJECT
public:
    PenEditorFactory(QObject *parent = 0);
    ~PenEditorFactory();

protected:
    void connectPropertyManager(PenPropertyManager *manager);
    QWidget *createEditor(PenPropertyManager *manager, QtProperty *property,
                          QWidget *parent);
    void disconnectPropertyManager(PenPropertyManager *manager);

private:
    QtEnumEditorFactory *m_comboBoxFactory;
    QtDoubleSpinBoxFactory *m_spinBoxFactory;
};

#endif // PENEDITORFACTORY_H
