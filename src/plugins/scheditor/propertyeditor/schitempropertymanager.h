#ifndef SCHITEMPROPERTYMANAGER_H
#define SCHITEMPROPERTYMANAGER_H

#include "qtpropertybrowser/qtvariantproperty.h"

class SchItemPropertyManager : public QtVariantPropertyManager
{
    Q_OBJECT
public:
    SchItemPropertyManager(QObject *parent = 0);
    ~SchItemPropertyManager();

};

#endif // SCHITEMPROPERTYMANAGER_H
