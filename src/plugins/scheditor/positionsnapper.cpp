#include "positionsnapper.h"
#include "schview.h"
#include "schscene.h"
#include "schitem.h"
#include "graphicsgrid.h"

#include <QAction>

/*******************************************************************************
 * Snap Strategy
 *******************************************************************************/

SnapStrategy::SnapStrategy(SchView *view):
    m_view(view),
    m_action(new QAction(nullptr))
{

}

QString SnapStrategy::name() const
{
    return m_name;
}

QString SnapStrategy::group() const
{
    return m_group;
}

void SnapStrategy::setGroup(const QString &name)
{
    m_group = name;
}

void SnapStrategy::setLabel(const QString &label)
{
    m_label = label;
}

void SnapStrategy::setName(const QString &name)
{
    m_name = name;
}

void SnapStrategy::setShortcut(const QKeySequence &shortcut)
{
    m_shortcut = shortcut;
}

void SnapStrategy::setIcon(const QIcon &icon)
{
    m_icon = icon;
}

void SnapStrategy::setSnappedPosition(QPoint pos)
{
    m_snappedPosition = pos;
}

void SnapStrategy::setSnappedItems(const QList<SchItem *> &items)
{
    m_snappedItems = items;
}

// TODO: don't include the item beeing edited/created
QList<SchItem *> SnapStrategy::itemsNearby(QPoint pos, qreal maxDistance)
{
    QPainterPath region;
    region.addEllipse(pos, maxDistance, maxDistance);
    QList<QGraphicsItem *> graphicsItems = view()->items(region, Qt::IntersectsItemShape);
    QList<SchItem *> schItems;
    foreach (QGraphicsItem *graphicsItem, graphicsItems) {
        // TODO: use type()
        SchItem *schItem = dynamic_cast<SchItem *>(graphicsItem);
        if (schItem != nullptr)
            schItems << schItem;
    }
    return schItems;
}

QPair<SchItem *, QPoint> SnapStrategy::closestItemPoint(QPoint pos, const QMultiMap<SchItem *, QPoint> &candidates)
{
    QPoint closestPoint;
    SchItem *closestItem = nullptr;
    int minDistance = INT_MAX;
    foreach (SchItem *item, candidates.keys()) {
        foreach (QPoint point, candidates.values(item)) {
            int distance = (point - pos).manhattanLength();
            if (distance < minDistance) {
                closestItem = item;
                closestPoint = point;
                minDistance = distance;
            }
        }
    }
    return qMakePair<SchItem *, QPoint>(closestItem, closestPoint);
}

void SnapStrategy::updateAction()
{
    m_action->setShortcut(m_shortcut);
    m_action->setToolTip(QString("%1 <b>%2</b>").arg(m_label).arg(m_shortcut.toString()));
    m_action->setIcon(m_icon);
    m_action->setCheckable(true);
}

QAction *SnapStrategy::action() const
{
    return m_action;
}

QIcon SnapStrategy::icon() const
{
    return m_icon;
}

QPainterPath SnapStrategy::decoration() const
{
    QPainterPath decoration;
    decoration.addEllipse(m_snappedPosition, 2, 2);
    decoration.addText(0, 25, QFont(), name());
    return decoration;
}

bool SnapStrategy::snap(QPoint mousePos, qreal maxDistance)
{
    Q_UNUSED(mousePos);
    Q_UNUSED(maxDistance);
    return false;
}

QList<SchItem *> SnapStrategy::snappedItems() const
{
    return m_snappedItems;
}

QPoint SnapStrategy::snappedPosition() const
{
    return m_snappedPosition;
}

SchView *SnapStrategy::view()
{
    return m_view;
}

/*******************************************************************************
 * Snap To Grid Strategy
 *******************************************************************************/

SnapToGridStrategy::SnapToGridStrategy(SchView *view):
    SnapStrategy(view)
{
    setLabel("Snap to grid");
    setName("Grid");
    setShortcut(QKeySequence("S,G"));
    setIcon(QIcon(":/icons/snap/Snap_Grid.svg"));
    updateAction();
}

bool SnapToGridStrategy::snap(QPoint mousePos, qreal maxDistance)
{
    Q_UNUSED(maxDistance);
    // TODO: foreach(GraphicsGrid *grid, view().grids()) { ... }
    // TODO: rename GraphicsGrid::snap() ?
    // TODO: or use the grid manager?
    GraphicsGrid *grid = view()->scene()->grid();
    QPoint pos = view()->mapFromScene(grid->snap(view()->pixelSize(), view()->mapToScene(mousePos)));
    setSnappedPosition(pos);
    return true;
}

/*******************************************************************************
 * Snap To Item's Hot Spots Strategy
 *******************************************************************************/

// TBD: snap to handles (hot spots), vs snap to ref points
SnapToItemHotSpotsStrategy::SnapToItemHotSpotsStrategy(SchView *view):
    SnapStrategy(view)
{
    setLabel("Snap to reference");
    setName("Reference");
    setShortcut(QKeySequence("S,R"));
    setIcon(QIcon(":/icons/snap/Snap_Angle.svg"));
    updateAction();
}

bool SnapToItemHotSpotsStrategy::snap(QPoint mousePos, qreal maxDistance)
{
    QMultiMap<SchItem*, QPoint> candidates;
    QPair<SchItem *, QPoint> winner;

    QList<SchItem*> items = itemsNearby(mousePos, maxDistance);
    foreach (SchItem *item, items) {
        foreach (QPointF hotSpot, item->hotSpots()) {
            QPoint pos = view()->mapFromScene(item->mapToScene(hotSpot));
            candidates.insert(item, pos);
        }
    }

    if (candidates.isEmpty())
        return false;

    winner = closestItemPoint(mousePos, candidates);
    setSnappedItems(QList<SchItem*>() << winner.first);
    setSnappedPosition(winner.second);
    return true;
}

/*******************************************************************************
 * Snap To Item's End Points Strategy
 *******************************************************************************/

SnapToItemEndPointStrategy::SnapToItemEndPointStrategy(SchView *view):
    SnapStrategy(view)
{
    setLabel("Snap to end points");
    setName("End point");
    setShortcut(QKeySequence("S,E"));
    setIcon(QIcon(":/icons/snap/Snap_Endpoint.svg"));
    updateAction();
}

bool SnapToItemEndPointStrategy::snap(QPoint mousePos, qreal maxDistance)
{
    QMultiMap<SchItem*, QPoint> candidates;
    QPair<SchItem *, QPoint> winner;

    QList<SchItem*> items = itemsNearby(mousePos, maxDistance);
    foreach (SchItem *item, items) {
        foreach (QPointF hotSpot, item->endPoints()) {
            QPoint pos = view()->mapFromScene(item->mapToScene(hotSpot));
            candidates.insert(item, pos);
        }
    }

    if (candidates.isEmpty())
        return false;

    winner = closestItemPoint(mousePos, candidates);
    setSnappedItems(QList<SchItem*>() << winner.first);
    setSnappedPosition(winner.second);
    return true;
}

/*******************************************************************************
 * Snap To Item's Mid Points Strategy
 *******************************************************************************/

SnapToItemMidPointStrategy::SnapToItemMidPointStrategy(SchView *view):
    SnapStrategy(view)
{
    setLabel("Snap to mid points");
    setName("Mid point");
    setShortcut(QKeySequence("S,M"));
    setIcon(QIcon(":/icons/snap/Snap_Midpoint.svg"));
    updateAction();
}

bool SnapToItemMidPointStrategy::snap(QPoint mousePos, qreal maxDistance)
{
    QMultiMap<SchItem*, QPoint> candidates;
    QPair<SchItem *, QPoint> winner;

    QList<SchItem*> items = itemsNearby(mousePos, maxDistance);
    foreach (SchItem *item, items) {
        foreach (QPointF hotSpot, item->midPoints()) {
            QPoint pos = view()->mapFromScene(item->mapToScene(hotSpot));
            candidates.insert(item, pos);
        }
    }

    if (candidates.isEmpty())
        return false;

    winner = closestItemPoint(mousePos, candidates);
    setSnappedItems(QList<SchItem*>() << winner.first);
    setSnappedPosition(winner.second);
    return true;
}

/*******************************************************************************
 * Snap To Item's Shape Points Strategy
 *******************************************************************************/

SnapToItemShapeStrategy::SnapToItemShapeStrategy(SchView *view):
    SnapStrategy(view)
{
    setLabel("Snap to shape points");
    setName("Shape point");
    setShortcut(QKeySequence("S,S"));
    setIcon(QIcon(":/icons/snap/Snap_WorkingPlane.svg"));
    updateAction();
}

bool SnapToItemShapeStrategy::snap(QPoint mousePos, qreal maxDistance)
{
    QMultiMap<SchItem*, QPoint> candidates;
    QPair<SchItem *, QPoint> winner;

    QList<SchItem*> items = itemsNearby(mousePos, maxDistance);
    foreach (SchItem *item, items) {
        QPointF itemPos = item->mapFromScene(view()->mapToScene(mousePos));
        foreach (QPointF itemPoint, item->nearestPoints(itemPos)) {
            QPoint pos = view()->mapFromScene(item->mapToScene(itemPoint));
            candidates.insert(item, pos);
        }
    }

    if (candidates.isEmpty())
        return false;

    winner = closestItemPoint(mousePos, candidates);
    setSnappedItems(QList<SchItem*>() << winner.first);
    setSnappedPosition(winner.second);
    return true;
}

/*******************************************************************************
 * Snap To Item's Center Points Strategy
 *******************************************************************************/

SnapToItemCenterStrategy::SnapToItemCenterStrategy(SchView *view):
    SnapStrategy(view)
{
    setLabel("Snap to center points");
    setName("Center point");
    setShortcut(QKeySequence("S,C"));
    setIcon(QIcon(":/icons/snap/Snap_Center.svg"));
    updateAction();
}

bool SnapToItemCenterStrategy::snap(QPoint mousePos, qreal maxDistance)
{
    QMultiMap<SchItem*, QPoint> candidates;
    QPair<SchItem *, QPoint> winner;

    QList<SchItem*> items = itemsNearby(mousePos, maxDistance);
    foreach (SchItem *item, items) {
        foreach (QPointF hotSpot, item->hotSpots()) {
            QPoint pos = view()->mapFromScene(item->mapToScene(hotSpot));
            candidates.insert(item, pos);
        }
    }

    if (candidates.isEmpty())
        return false;

    winner = closestItemPoint(mousePos, candidates);
    setSnappedItems(QList<SchItem*>() << winner.first);
    setSnappedPosition(winner.second);
    return true;
}

/*******************************************************************************
 * Snap Manager
 *******************************************************************************/

SnapManager::SnapManager(SchView *view):
    QObject(view)
{
    m_strategies << new SnapToGridStrategy(view)
                 << new SnapToItemHotSpotsStrategy(view)
                 << new SnapToItemEndPointStrategy(view)
                 << new SnapToItemMidPointStrategy(view)
                 << new SnapToItemCenterStrategy(view)
                 << new SnapToItemShapeStrategy(view);
    // TODO: connect action to enabled/disabled
    foreach (const QString &group, groups()) {
        QActionGroup *actionGroup = new QActionGroup(this);
        foreach (QAction *action, actions(group)) {
            action->setActionGroup(actionGroup);
        }
    }
}

QList<QString> SnapManager::groups() const
{
    QList<QString> result;
    foreach (SnapStrategy *strategy, m_strategies) {
        if (!result.contains(strategy->group()))
            result.append(strategy->group());
    }
    return result;
}

QList<QAction *> SnapManager::actions(const QString &group) const
{
    QList<QAction *> result;
    foreach (SnapStrategy *strategy, m_strategies) {
        if (group.isEmpty() || strategy->group() == group)
            result.append(strategy->action());
    }
    return result;
}

QPainterPath SnapManager::decoration() const
{
    Q_ASSERT(m_winnerStrategy != nullptr);
    return m_winnerStrategy->decoration();
}

bool SnapManager::snap(QPoint mousePos, qreal maxDistance)
{
    m_winnerStrategy = nullptr;
    int minDistance = INT_MAX;
    foreach (SnapStrategy *strategy, m_strategies) {
        // Todo: only if enabled
        if (strategy->snap(mousePos, maxDistance)) {
            int distance = (strategy->snappedPosition() - mousePos).manhattanLength();
            if (distance < minDistance) {
                m_winnerStrategy = strategy;
                minDistance = distance;
            }
        }
    }
    return m_winnerStrategy != nullptr;
}

QList<SchItem *> SnapManager::snappedItems() const
{
    Q_ASSERT(m_winnerStrategy != nullptr);
    return m_winnerStrategy->snappedItems();
}

QPoint SnapManager::snappedPosition() const
{
    Q_ASSERT(m_winnerStrategy != nullptr);
    return m_winnerStrategy->snappedPosition();
}

SnapStrategy *SnapManager::snappingStrategy() const
{
    return m_winnerStrategy;
}
