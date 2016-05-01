#ifndef SCHITEMPROPERTYMANAGER_H
#define SCHITEMPROPERTYMANAGER_H

#include "qtpropertybrowser/qtvariantproperty.h"

class SchItemPropertyManager : public QtVariantPropertyManager
{
    Q_OBJECT
public:
    explicit SchItemPropertyManager(QObject *parent = nullptr);
    ~SchItemPropertyManager();

};

#endif // SCHITEMPROPERTYMANAGER_H
