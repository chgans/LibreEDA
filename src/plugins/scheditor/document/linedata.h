#ifndef LINEDATA_H
#define LINEDATA_H

#include "document/graphicsitemdata.h"

class LineData: public GraphicsItemData
{
    Q_GADGET

    Q_PROPERTY(QPointF p1 READ p1 WRITE setP1)
    Q_PROPERTY(QPointF p2 READ p2 WRITE setP2)

    QPointF m_p1;
    QPointF m_p2;

public:
    LineData();

    QPointF p1() const
    {
        return m_p1;
    }

    QPointF p2() const
    {
        return m_p2;
    }

public slots:
    void setP1(QPointF p1)
    {
        m_p1 = p1;
    }

    void setP2(QPointF p2)
    {
        m_p2 = p2;
    }
};

#endif // LINEDATA_H
