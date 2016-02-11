#ifndef IOUTPUTPANEFACTORY_H
#define IOUTPUTPANEFACTORY_H

#include "core/core_global.h"

#include <QObject>

class CORE_EXPORT IOutputPaneFactory : public QObject
{
    Q_OBJECT
public:
    explicit IOutputPaneFactory(QObject *parent = 0);
    virtual ~IOutputPaneFactory();

    void setDisplayName(const QString &displayName);
    void setPriority(int priority);

    QString displayName() const;
    int priority() const;

    virtual QWidget *createWidget() = 0;

private:
    QString m_displayName;
    int m_priority;
};

#endif // IOUTPUTPANEFACTORY_H
