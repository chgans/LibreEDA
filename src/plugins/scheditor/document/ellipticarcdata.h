#ifndef ELLIPTICARCDATA_H
#define ELLIPTICARCDATA_H

#include "document/graphicsitemdata.h"

class EllipticArcData: public GraphicsItemData
{
    Q_GADGET

    Q_PROPERTY(qreal xRadius READ xRadius WRITE setXRadius)
    Q_PROPERTY(qreal yRadius READ yRadius WRITE setYRadius)
    Q_PROPERTY(qreal startAngle READ startAngle WRITE setStartAngle)
    Q_PROPERTY(qreal spanAngle READ spanAngle WRITE setSpanAngle)

    qreal m_xRadius;
    qreal m_yRadius;
    qreal m_startAngle;
    qreal m_spanAngle;

public:
    EllipticArcData();

    qreal xRadius() const
    {
        return m_xRadius;
    }

    qreal yRadius() const
    {
        return m_yRadius;
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
    void setXRadius(qreal xRadius)
    {
        m_xRadius = xRadius;
    }

    void setYRadius(qreal yRadius)
    {
        m_yRadius = yRadius;
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

#endif // ELLIPTICARCDATA_H
