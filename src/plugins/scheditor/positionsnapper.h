#ifndef POSITIONSNAPPER_H
#define POSITIONSNAPPER_H

#include <QPointF>
#include <QLineF>
#include <QPainterPath>

class SchView;
class QGraphicsLineItem;
class PositionSnapper
{
public:
    PositionSnapper(SchView *view);

    QPointF constrains(QPointF point, QLineF axis) const;
    QPoint snap(QPoint point);

    void setNearDistance(qreal distance);
    void setFarDistance(qreal distance);

private:
    // Hot spots
    bool m_snapToObjectHotSpots;
    bool m_snapToNearObjects;
    qreal m_nearDistance;
    QPainterPath m_nearRegion;
    bool m_snapToFarObjects;
    qreal m_farDistance;
    QPainterPath m_farRegion;

    // Axis
    bool m_snapToObjectAxis;
    mutable QList<QGraphicsLineItem *> m_axisItemList;

    // Grids
    bool m_snapToGrid;

    // Guides
    bool m_snapToGuide;

    //
    SchView *m_view;

    QPoint m_oldPos;
};

#endif // POSITIONSNAPPER_H
