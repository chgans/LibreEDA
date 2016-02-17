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

    QtEnumEditorFactory *subEnumEditorFactory();
    QtColorEditorFactory *subColorEditorFactory();

protected:
    void connectPropertyManager(PenPropertyManager *manager);
    QWidget *createEditor(PenPropertyManager *manager, QtProperty *property,
                          QWidget *parent);
    void disconnectPropertyManager(PenPropertyManager *manager);

private:
    QtEnumEditorFactory *m_comboBoxFactory;
    QtColorEditorFactory *m_colorFactory;
};

#endif // PENEDITORFACTORY_H
