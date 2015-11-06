#ifndef POLYGONDATA_H
#define POLYGONDATA_H

#include "document/graphicsitemdata.h"

class PolygonData: public GraphicsItemData
{
    Q_GADGET

    Q_PROPERTY(QList<QPointF> points READ points WRITE setPoints)
    Q_PROPERTY(bool isClosed READ isClosed WRITE setIsClosed)

    QList<QPointF> m_points;
    bool m_isClosed;

public:
    PolygonData();

    QList<QPointF> points() const
    {
        return m_points;
    }

    bool isClosed() const
    {
        return m_isClosed;
    }

public slots:
    void setPoints(QList<QPointF> points)
    {
        m_points = points;
    }

    void setIsClosed(bool isClosed)
    {
        m_isClosed = isClosed;
    }
};

#endif // POLYGONDATA_H
