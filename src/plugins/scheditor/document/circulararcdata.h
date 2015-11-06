#ifndef CIRCULARARCDATA_H
#define CIRCULARARCDATA_H

#include "document/graphicsitemdata.h"

class CircularArcData: public GraphicsItemData
{
    Q_GADGET

    Q_PROPERTY(qreal radius READ radius WRITE setRadius)
    Q_PROPERTY(qreal startAngle READ startAngle WRITE setStartAngle)
    Q_PROPERTY(qreal spanAngle READ spanAngle WRITE setSpanAngle)

    qreal m_radius;
    qreal m_startAngle;
    qreal m_spanAngle;

public:
    CircularArcData();

    qreal radius() const
    {
        return m_radius;
    }

    qreal startAngle() const
    {
        return m_startAngle;
    }

    qreal spanAngle() const
    {
        return m_spanAngle;
    }

public slots:
    void setRadius(qreal radius)
    {
        m_radius = radius;
    }

    void setStartAngle(qreal startAngle)
    {
        m_startAngle = startAngle;
    }

    void setSpanAngle(qreal spanAngle)
    {
        m_spanAngle = spanAngle;
    }
};

#endif // CIRCULARARCDATA_H
