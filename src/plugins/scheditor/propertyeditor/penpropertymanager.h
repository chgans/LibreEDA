#ifndef PENPROPERTYMANAGER_H
#define PENPROPERTYMANAGER_H

#include "qtpropertybrowser/qtpropertymanager.h"

#include <QMap>
#include <QPen>

class PenPropertyManager : public QtAbstractPropertyManager
{
    Q_OBJECT
public:
    explicit PenPropertyManager(QObject *parent = nullptr);
    ~PenPropertyManager();

    QtEnumPropertyManager *subEnumManager() const;
    QtColorPropertyManager *subColorManager() const;

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
    QtColorPropertyManager *m_colorManager;

    QMap<const QtProperty *, QtProperty *> m_styleToPenProperty;
    QMap<const QtProperty *, QtProperty *> m_penToStyleProperty;
    QMap<const QtProperty *, QtProperty *> m_widthToPenProperty;
    QMap<const QtProperty *, QtProperty *> m_penToWidthProperty;
    QMap<const QtProperty *, QtProperty *> m_colorToPenProperty;
    QMap<const QtProperty *, QtProperty *> m_penToColorProperty;

private slots:
    void slotEnumChanged(QtProperty *property, int val);
    void slotColorChanged(QtProperty *property, const QColor &val);

    void slotPropertyDestroyed(QtProperty *property);
};

#endif // PENPROPERTYMANAGER_H
