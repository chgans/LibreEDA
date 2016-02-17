#ifndef PENPROPERTYMANAGER_H
#define PENPROPERTYMANAGER_H

#include "qtpropertybrowser/qtpropertymanager.h"

#include <QMap>
#include <QPen>

class PenPropertyManager : public QtAbstractPropertyManager
{
    Q_OBJECT
public:
    PenPropertyManager(QObject *parent = 0);
    ~PenPropertyManager();

    QtEnumPropertyManager *subEnumManager() const;
    QtDoublePropertyManager *subDoubleManager() const;

    QPen value(const QtProperty *property) const;

public slots:
    void setValue(QtProperty *property, const QPen &val);

signals:
    void valueChanged(QtProperty *property, const QPen &val);

protected:
    QString valueText(const QtProperty *property) const;
    virtual void initializeProperty(QtProperty *property);
    virtual void uninitializeProperty(QtProperty *property);

private:
    QtEnumPropertyManager *m_enumManager;
    QtDoublePropertyManager *m_doubleManager;
    QMap<const QtProperty *, QtProperty *> m_styleToPenProperty;
    QMap<const QtProperty *, QtProperty *> m_penToStyleProperty;
    QMap<const QtProperty *, QtProperty *> m_widthToPenProperty;
    QMap<const QtProperty *, QtProperty *> m_penToWidthProperty;

    void slotEnumChanged(QtProperty *property, int val);
    void slotPropertyDestroyed(QtProperty *property);
};

#endif // PENPROPERTYMANAGER_H
