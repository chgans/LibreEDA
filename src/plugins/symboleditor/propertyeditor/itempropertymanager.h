#pragma once

#include "qtpropertybrowser/qtpropertybrowser.h"
#include "qtpropertybrowser/qtpropertymanager.h"

namespace SymbolEditor
{

    class ItemPropertyManager : public QtAbstractPropertyManager
    {
        Q_OBJECT
    public:
        explicit ItemPropertyManager(QObject *parent = nullptr);
        ~ItemPropertyManager();

    private:
        QtDoublePropertyManager *m_realPropertyManager;
        QtBoolPropertyManager *m_boolPropertyManager;
    };

}
