#ifndef CIRCLEDATA_H
#define CIRCLEDATA_H

#include "document/graphicsitemdata.h"

class CircleData: public GraphicsItemData
{
    Q_GADGET

    Q_PROPERTY(qreal radius READ radius WRITE setRadius)
    qreal m_radius;

public:
    CircleData();

    qreal radius() const
    {
        return m_radius;
    }

public slots:
    void setRadius(qreal radius)
    {
        m_radius = radius;
    }
};

#endif // CIRCLEDATA_H
