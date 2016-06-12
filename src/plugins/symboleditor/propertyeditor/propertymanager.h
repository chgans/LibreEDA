#pragma once

#include <QObject>
#include <QMap>

class QtProperty;
class QtAbstractPropertyManager;
class QtBoolPropertyManager;
class QtDoublePropertyManager;
class QtIntPropertyManager;
class QtPointFPropertyManager;
class QtEnumPropertyManager;
class QtStringPropertyManager;
class QtAbstractPropertyBrowser;

namespace SymbolEditor
{

    class PropertyManager: public QObject
    {
        Q_OBJECT

    public:
        explicit PropertyManager(QObject *parent = nullptr);
        ~PropertyManager();

        void setBrowserFactories(QtAbstractPropertyBrowser *browser);

        void addCoordinate(quint64 id, const QString &name);
        void addLength(quint64 id, const QString &name);
        void addAngle(quint64 id, const QString &name);
        void addPercentage(quint64 id, const QString &name);
        void addColor(quint64 id, const QString &name);
        void addBistate(quint64 id, const QString &name);
        void addText(quint64 id, const QString &name);
        void addLineStyle(quint64 id, const QString &name);
        void addLineWidth(quint64 id, const QString &name);
        void addFillStyle(quint64 id, const QString &name);
        void clear();

        QVariant value(quint64 id) const;
        void setValue(quint64 id, const QVariant &value);

        QString name(quint64 id) const;
        void setName(quint64 id, const QString &name);
        QString toolTip(quint64 id) const;
        void setToolTip(quint64 id, const QString &toolTip);
        QString statusTip(quint64 id) const;
        void setStatusTip(quint64 id, const QString &statusTip);
        QString whatsThis(quint64 id) const;
        void setWhatsThis(quint64 id, const QString &whatsThis);
        bool isEnabled(quint64 id) const;
        void setEnabled(quint64 id, bool enable);
        bool isModified(quint64 id) const;
        void setModified(quint64 id, bool modified);

    signals:
        void valueChanged(quint64 id, const QVariant &value);
        void propertyAdded(QtProperty *property);

    private slots:
        void onEnumValueChanged(QtProperty *property, int value);
        void onPointValueChanged(QtProperty *property, const QPointF &value);
        void onRealValueChanged(QtProperty *property, qreal value);
        void onIntValueChanged(QtProperty *property, int value);
        void onBoolValueChanged(QtProperty *property, bool value);
        void onStringValueChanged(QtProperty *property, const QString &value);

    private:
        QtEnumPropertyManager *m_enumManager;
        QtPointFPropertyManager *m_pointManager;
        QtDoublePropertyManager *m_realManager;
        QtIntPropertyManager *m_intManager;
        QtBoolPropertyManager *m_boolManager;
        QtStringPropertyManager *m_stringManager;

        void addProperty(quint64 id, QtProperty *property, QtAbstractPropertyManager *manager);
        QMap<QtProperty*, quint64> m_propertyToIdMap;
        QMap<quint64, QtProperty*> m_idToPropertyMap;
        QMap<quint64, QtAbstractPropertyManager*> m_idToManagerMap;
    };

}
