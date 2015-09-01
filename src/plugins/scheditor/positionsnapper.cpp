#include "positionsnapper.h"
#include "schview.h"
#include "schscene.h"
#include "schitem.h"
#include "graphicsgrid.h"

#include <QDebug>

PositionSnapper::PositionSnapper(SchView *view):
    m_view(view)
{
    setNearDistance(100);
    setFarDistance(250);
}

/*
 * Priorities:
 *  - axes
 *  - hot spots
 *  - guides
 *  - grids
 */
QPoint PositionSnapper::snap(QPoint point)
{
    // Snap points candidates
    QList<QPoint> candidates;

    // Snap to grid
    QPoint p = m_view->mapFromScene(m_view->scene()->grid()->snap(m_view->pixelSize(), m_view->mapToScene(point)));
    candidates.append(p);

    // Snap to hot spots
    QList<QGraphicsItem*> items = m_view->items(m_nearRegion.translated(point));
    foreach (QGraphicsItem *i, items) {
        SchItem *item = dynamic_cast<SchItem*>(i); // TODO: use item.type()
        if (item == nullptr)
            continue;
        foreach (QPointF hotSpot, item->hotSpots()) {
            candidates.append(m_view->mapFromScene(item->mapToScene(hotSpot)));
        }
    }

    // Pick the closest candidate
    QPoint snapPos = point;
    int min = INT_MAX;
    foreach (QPoint candidate, candidates) {
        int length = (candidate - point).manhattanLength();
        if (min > length) {
            snapPos = candidate;
            min = length;
        }
    }
    m_oldPos = snapPos;
    return snapPos;
}

void PositionSnapper::setNearDistance(qreal distance)
{
    m_nearDistance = distance;
    m_nearRegion = QPainterPath();
    m_nearRegion.addEllipse(QPointF(0, 0), m_nearDistance, m_nearDistance);
}

void PositionSnapper::setFarDistance(qreal distance)
{
    m_farDistance = distance;
    m_farRegion = QPainterPath();
    m_farRegion.addEllipse(QPointF(0, 0), m_farDistance, m_farDistance);
}

