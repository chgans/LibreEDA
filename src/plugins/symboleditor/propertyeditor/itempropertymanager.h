#pragma once

#include <QObject>
#include <QPen>

class QtProperty;
class QtBoolPropertyManager;
class QtDoublePropertyManager;
class QtPointFPropertyManager;
class QtAbstractPropertyBrowser;

namespace SymbolEditor
{

    class PenPropertyManager;

    class ItemPropertyManager: public QObject
    {
        Q_OBJECT
    public:
        explicit ItemPropertyManager(QObject *parent = nullptr);
        ~ItemPropertyManager();

        void setBrowserFactories(QtAbstractPropertyBrowser *browser);

        QtProperty *addRealProperty(const QString &name, qreal start, qreal step, qreal end);
        QtProperty *addBoolProperty(const QString &name);
        QtProperty *addPointProperty(const QString &name);
        QtProperty *addPenProperty(const QString &name);
        void clear();

        void setPropertyValue(QtProperty *property, qreal value);
        void setPropertyValue(QtProperty *property, bool value);
        void setPropertyValue(QtProperty *property, const QPointF &point);
        void setPropertyValue(QtProperty *property, QPen pen);

    signals:
        void realValueChanged(QtProperty *property, qreal value);
        void boolValueChanged(QtProperty *property, bool value);
        void pointValueChanged(QtProperty *property, const QPointF &point);
        void penValueChanged(QtProperty *property, QPen pen);

    private:
        QtPointFPropertyManager *m_pointManager;
        QtDoublePropertyManager *m_realManager;
        QtBoolPropertyManager *m_boolManager;
        PenPropertyManager *m_penManager;
    };

}
