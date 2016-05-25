#ifndef PENEDITORFACTORY_H
#define PENEDITORFACTORY_H

#include "qtpropertybrowser/qteditorfactory.h"
#include "penpropertymanager.h"

namespace SymbolEditor
{

    class PenEditorFactory: public QtAbstractEditorFactory<PenPropertyManager>
    {
        Q_OBJECT
    public:
        explicit PenEditorFactory(QObject *parent = nullptr);
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

}

#endif // PENEDITORFACTORY_H
