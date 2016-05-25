#pragma once

#include "qtpropertybrowser/qttreepropertybrowser.h"

#include <QMap>

class QtVariantPropertyManager;
class QtVariantEditorFactory;
class QtProperty;

namespace SymbolEditor
{

    class PenPropertyManager;
    class PenEditorFactory;

    class ObjectPropertyEditor: public QtTreePropertyBrowser
    {
        Q_OBJECT

    public:
        explicit ObjectPropertyEditor(QWidget *parent = nullptr);
        ~ObjectPropertyEditor();

    public slots:
        void setObjects(QList<QObject *> objects);

    private:
        QtVariantPropertyManager *m_manager;
        QtVariantEditorFactory *m_factory;
        PenPropertyManager *m_penManager;
        PenEditorFactory *m_penFactory;
        QObject *m_object;
        QMap<QtProperty *, int> m_propertyToMetaPropertyIndex;

        bool m_populatingBrowser;

        void populateBrowser(QObject *object, const QMetaObject *metaObject);


    private slots:
        void setObjectPropertyValue(QtProperty *property, const QVariant &value);
        void setBrowserPropertyValue(QtProperty *property, const QVariant &value);
    };

}
