#pragma once

#include "itempropertyadapter.h"

class QtProperty;

namespace SymbolEditor
{

    class CirclePropertyAdapter : public ItemPropertyAdapter
    {
        Q_OBJECT

    public:
        explicit CirclePropertyAdapter(QObject *parent = 0);

        QList<QtProperty *> properties() const;

    signals:
        void taskCompleted(UndoCommand *command);

    public slots:


    protected:
        void createProperties(ItemPropertyManager *manager);
        void updateProperties(ItemPropertyManager *manager);
        void deleteProperties(ItemPropertyManager *manager);

    protected slots:
        void onBoolValueChanged(QtProperty *property, bool value);
        void onRealValueChanged(QtProperty *property, qreal value);

    private:
        QtProperty *m_center;
        QtProperty *m_radius;
        QtProperty *m_pen;
        QtProperty *m_brush;
    };

}
