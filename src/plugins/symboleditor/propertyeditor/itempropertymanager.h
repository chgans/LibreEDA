#pragma once

#include <QObject>

class QtProperty;
class QtBoolPropertyManager;
class QtDoublePropertyManager;
class QtAbstractPropertyBrowser;

namespace SymbolEditor
{

    class ItemPropertyManager: public QObject
    {
        Q_OBJECT
    public:
        explicit ItemPropertyManager(QObject *parent = nullptr);
        ~ItemPropertyManager();

        void setBrowserFactories(QtAbstractPropertyBrowser *browser);

        QtProperty *addRealProperty(const QString &name, qreal start, qreal step, qreal end);
        QtProperty *addBoolProperty(const QString &name);

        void setPropertyValue(QtProperty *property, qreal value);
        void setPropertyValue(QtProperty *property, bool value);

    signals:
        void realValueChanged(QtProperty *property, qreal value);
        void boolValueChanged(QtProperty *property, bool value);

    private:
        QtDoublePropertyManager *m_realPropertyManager;
        QtBoolPropertyManager *m_boolPropertyManager;
    };

}
