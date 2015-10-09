#ifndef ELLIPSEDATA_H
#define ELLIPSEDATA_H

#include "document/graphicsitemdata.h"

class EllipseData: public GraphicsItemData
{
    Q_GADGET

    Q_PROPERTY(qreal xRadius READ xRadius WRITE setXRadius)
    Q_PROPERTY(qreal yRadius READ yRadius WRITE setYRadius)

    qreal m_xRadius;
    qreal m_yRadius;

public:
    EllipseData();

    qreal xRadius() const
    {
        return m_xRadius;
    }

    qreal yRadius() const
    {
        return m_yRadius;
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
};

#endif // ELLIPSEDATA_H
