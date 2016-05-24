#include "snap/positionsnapper.h"
#include "view.h"
#include "scene.h"
#include "item/item.h"
#include "grid/grid.h"

#include <QAction>

/*******************************************************************************
 * Snap Strategy
 *******************************************************************************/

SnapStrategy::SnapStrategy(View *view):
    m_view(view),
    m_action(new QAction(nullptr))
{

}

SnapStrategy::~SnapStrategy()
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

void SnapStrategy::setSnappedPosition(QPointF pos)
{
    m_snappedPosition = pos;
}

void SnapStrategy::setSnappedItems(const QList<Item *> &items)
{
    m_snappedItems = items;
}

// TODO: don't include the item beeing edited/created
QList<Item *> SnapStrategy::itemsNearby(QPointF pos, qreal maxDistance)
{
    QPainterPath region;
    region.addEllipse(pos, maxDistance, maxDistance);
    QList<QGraphicsItem *> graphicsItems = view()->scene()->items(region, Qt::IntersectsItemShape);
    QList<Item *> schItems;
    for (QGraphicsItem *graphicsItem : graphicsItems)
    {
        // TODO: use type()
        Item *schItem = dynamic_cast<Item *>(graphicsItem);
        if (schItem != nullptr)
        {
            schItems << schItem;
        }
    }
    return schItems;
}

QPair<Item *, QPointF> SnapStrategy::closestItemPoint(QPointF pos,
                                                         const QMultiMap<Item *, QPointF> &candidates)
{
    QPointF closestPoint;
    Item *closestItem = nullptr;
    int minDistance = INT_MAX;
    for (Item *item : candidates.keys())
    {
        for (QPointF point : candidates.values(item))
        {
            int distance = (point - pos).manhattanLength();
            if (distance < minDistance)
            {
                closestItem = item;
                closestPoint = point;
                minDistance = distance;
            }
        }
    }
    return qMakePair<Item *, QPointF>(closestItem, closestPoint);
}

void SnapStrategy::updateAction()
{
    m_action->setShortcut(m_shortcut);
    m_action->setToolTip(QString("%1 <b>%2</b>").arg(m_label).arg(m_shortcut.toString()));
    m_action->setIcon(m_icon);
    m_action->setCheckable(true);
    m_action->setChecked(false);
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
    //decoration.addEllipse(QPoint(0, 0), 2, 2);
    decoration.addText(QPoint(10, 25), QFont(), name());
    return decoration;
}

bool SnapStrategy::snap(QPointF mousePos, qreal maxDistance)
{
    Q_UNUSED(mousePos);
    Q_UNUSED(maxDistance);
    return false;
}

QList<Item *> SnapStrategy::snappedItems() const
{
    return m_snappedItems;
}

QPointF SnapStrategy::snappedPosition() const
{
    return m_snappedPosition;
}

bool SnapStrategy::isEnabled() const
{
    return m_action->isChecked();
}

View *SnapStrategy::view()
{
    return m_view;
}

/*******************************************************************************
 * No Snap Strategy (Free)
 *******************************************************************************/

NoSnapStrategy::NoSnapStrategy(View *view):
    SnapStrategy(view)
{
    setLabel("No snap");
    setName("Free");
    setGroup("leda.snap.default");
    setShortcut(QKeySequence("S,F"));
    setIcon(QIcon(":/icons/snap/Snap_Free.svg"));
    updateAction();
}

bool NoSnapStrategy::snap(QPointF mousePos, qreal maxDistance)
{
    Q_UNUSED(maxDistance);
    setSnappedPosition(mousePos);
    return true;
}

/*******************************************************************************
 * Snap To Grid Strategy
 *******************************************************************************/

SnapToGridStrategy::SnapToGridStrategy(View *view):
    SnapStrategy(view)
{
    setLabel("Snap to grid");
    setName("Grid");
    setGroup("leda.snap.default");
    setShortcut(QKeySequence("S,G"));
    setIcon(QIcon(":/icons/snap/Snap_Grid.svg"));
    updateAction();
}

bool SnapToGridStrategy::snap(QPointF mousePos, qreal maxDistance)
{
    Q_UNUSED(maxDistance);
    // TODO: for (GraphicsGrid *grid: view().grids()) { ... }
    // TODO: rename GraphicsGrid::snap() ?
    // TODO: or use the grid manager?
    const Grid *grid = view()->grid();
    QPointF pos = grid->snap(view()->pixelSize(), mousePos);
    setSnappedPosition(pos);
    return true;
}

/*******************************************************************************
 * Snap To Item's Hot Spots Strategy
 *******************************************************************************/

// TBD: snap to handles (hot spots), vs snap to ref points
SnapToItemHotSpotsStrategy::SnapToItemHotSpotsStrategy(View *view):
    SnapStrategy(view)
{
    setLabel("Snap to reference");
    setName("Reference");
    setGroup("leda.snap.default");
    setShortcut(QKeySequence("S,R"));
    setIcon(QIcon(":/icons/snap/Snap_Angle.svg"));
    updateAction();
}

bool SnapToItemHotSpotsStrategy::snap(QPointF mousePos, qreal maxDistance)
{
    QMultiMap<Item *, QPointF> candidates;
    QPair<Item *, QPointF> winner;

    QList<Item *> items = itemsNearby(mousePos, maxDistance);
    for (Item *item : items)
    {
        for (QPointF hotSpot : item->hotSpots())
        {
            QPointF pos = item->mapToScene(hotSpot);
            candidates.insert(item, pos);
        }
    }

    if (candidates.isEmpty())
    {
        return false;
    }

    winner = closestItemPoint(mousePos, candidates);
    setSnappedItems(QList<Item *>() << winner.first);
    setSnappedPosition(winner.second);
    return true;
}

/*******************************************************************************
 * Snap To Item's End Points Strategy
 *******************************************************************************/

SnapToItemEndPointStrategy::SnapToItemEndPointStrategy(View *view):
    SnapStrategy(view)
{
    setLabel("Snap to end points");
    setName("End point");
    setGroup("leda.snap.default");
    setShortcut(QKeySequence("S,E"));
    setIcon(QIcon(":/icons/snap/Snap_Endpoint.svg"));
    updateAction();
}

bool SnapToItemEndPointStrategy::snap(QPointF mousePos, qreal maxDistance)
{
    QMultiMap<Item *, QPointF> candidates;
    QPair<Item *, QPointF> winner;

    QList<Item *> items = itemsNearby(mousePos, maxDistance);
    for (Item *item : items)
    {
        for (QPointF hotSpot : item->endPoints())
        {
            QPointF pos = item->mapToScene(hotSpot);
            candidates.insert(item, pos);
        }
    }

    if (candidates.isEmpty())
    {
        return false;
    }

    winner = closestItemPoint(mousePos, candidates);
    setSnappedItems(QList<Item *>() << winner.first);
    setSnappedPosition(winner.second);
    return true;
}

/*******************************************************************************
 * Snap To Item's Mid Points Strategy
 *******************************************************************************/

SnapToItemMidPointStrategy::SnapToItemMidPointStrategy(View *view):
    SnapStrategy(view)
{
    setLabel("Snap to mid points");
    setName("Mid point");
    setGroup("leda.snap.default");
    setShortcut(QKeySequence("S,M"));
    setIcon(QIcon(":/icons/snap/Snap_Midpoint.svg"));
    updateAction();
}

bool SnapToItemMidPointStrategy::snap(QPointF mousePos, qreal maxDistance)
{
    QMultiMap<Item *, QPointF> candidates;
    QPair<Item *, QPointF> winner;

    QList<Item *> items = itemsNearby(mousePos, maxDistance);
    for (Item *item : items)
    {
        for (QPointF hotSpot : item->midPoints())
        {
            QPointF pos = item->mapToScene(hotSpot);
            candidates.insert(item, pos);
        }
    }

    if (candidates.isEmpty())
    {
        return false;
    }

    winner = closestItemPoint(mousePos, candidates);
    setSnappedItems(QList<Item *>() << winner.first);
    setSnappedPosition(winner.second);
    return true;
}

/*******************************************************************************
 * Snap To Item's Shape Points Strategy
 *******************************************************************************/

SnapToItemShapeStrategy::SnapToItemShapeStrategy(View *view):
    SnapStrategy(view)
{
    setLabel("Snap to shape points");
    setName("Shape point");
    setGroup("leda.snap.default");
    setShortcut(QKeySequence("S,S"));
    setIcon(QIcon(":/icons/snap/Snap_WorkingPlane.svg"));
    updateAction();
}

bool SnapToItemShapeStrategy::snap(QPointF mousePos, qreal maxDistance)
{
    QMultiMap<Item *, QPointF> candidates;
    QPair<Item *, QPointF> winner;

    QList<Item *> items = itemsNearby(mousePos, maxDistance);
    for (Item *item : items)
    {
        QPointF itemPos = item->mapFromScene(mousePos);
        for (QPointF itemPoint : item->nearestPoints(itemPos))
        {
            QPointF pos = item->mapToScene(itemPoint);
            candidates.insert(item, pos);
        }
    }

    if (candidates.isEmpty())
    {
        return false;
    }

    winner = closestItemPoint(mousePos, candidates);
    setSnappedItems(QList<Item *>() << winner.first);
    setSnappedPosition(winner.second);
    return true;
}

/*******************************************************************************
 * Snap To Item's Center Points Strategy
 *******************************************************************************/

SnapToItemCenterStrategy::SnapToItemCenterStrategy(View *view):
    SnapStrategy(view)
{
    setLabel("Snap to center points");
    setName("Center point");
    setGroup("leda.snap.default");
    setShortcut(QKeySequence("S,C"));
    setIcon(QIcon(":/icons/snap/Snap_Center.svg"));
    updateAction();
}

bool SnapToItemCenterStrategy::snap(QPointF mousePos, qreal maxDistance)
{
    QMultiMap<Item *, QPointF> candidates;
    QPair<Item *, QPointF> winner;

    QList<Item *> items = itemsNearby(mousePos, maxDistance);
    for (Item *item : items)
    {
        for (QPointF center : item->centerPoints())
        {
            QPointF pos = item->mapToScene(center);
            candidates.insert(item, pos);
        }
    }

    if (candidates.isEmpty())
    {
        return false;
    }

    winner = closestItemPoint(mousePos, candidates);
    setSnappedItems(QList<Item *>() << winner.first);
    setSnappedPosition(winner.second);
    return true;
}

/*******************************************************************************
 * Snap Manager
 *******************************************************************************/

// TODO: Add auto strategy:
//Snaps to all end points, intersection points, middle points,
//reference points and grid points in this order of priority.

SnapManager::SnapManager(View *view):
    QObject(view)
{
    m_defaultStrategy = new NoSnapStrategy(view);
    m_strategies << m_defaultStrategy
                 << new SnapToGridStrategy(view)
                 << new SnapToItemHotSpotsStrategy(view)
                 << new SnapToItemEndPointStrategy(view)
                 << new SnapToItemMidPointStrategy(view)
                 << new SnapToItemCenterStrategy(view)
                 << new SnapToItemShapeStrategy(view);

    for (const QString &group : groups())
    {
        QActionGroup *actionGroup = new QActionGroup(this);
        actionGroup->setExclusive(true);
        for (QAction *action : actions(group))
        {
            action->setActionGroup(actionGroup);
        }
    }
    m_defaultStrategy->action()->setChecked(true);
    m_winnerStrategy = m_defaultStrategy;
}

SnapManager::~SnapManager()
{
    qDeleteAll(m_strategies);
}

QList<QString> SnapManager::groups() const
{
    QList<QString> result;
    for (SnapStrategy *strategy : m_strategies)
    {
        if (!result.contains(strategy->group()))
        {
            result.append(strategy->group());
        }
    }
    return result;
}

QList<QAction *> SnapManager::actions(const QString &group) const
{
    QList<QAction *> result;
    for (SnapStrategy *strategy : m_strategies)
    {
        if (group.isEmpty() || strategy->group() == group)
        {
            result.append(strategy->action());
        }
    }
    return result;
}

QPainterPath SnapManager::decoration() const
{
    Q_ASSERT(m_winnerStrategy != nullptr);
    if (m_winnerStrategy != m_defaultStrategy)
    {
        return m_winnerStrategy->decoration();
    }
    else
    {
        return QPainterPath();    // Or make NoSnapStrategy returns an empty decoration?
    }
}

bool SnapManager::snap(QPointF mousePos, qreal maxDistance)
{
    m_winnerStrategy = nullptr;
    qreal minDistance = std::numeric_limits<qreal>::max();
    for (SnapStrategy *strategy : m_strategies)
    {
        if (!strategy->isEnabled())
        {
            continue;
        }
        if (!strategy->snap(mousePos, maxDistance))
        {
            continue;
        }
        qreal distance = (strategy->snappedPosition() - mousePos).manhattanLength();
        if (distance < minDistance)
        {
            m_winnerStrategy = strategy;
            minDistance = distance;
        }
    }
    return m_winnerStrategy != nullptr;
}

QList<Item *> SnapManager::snappedItems() const
{
    Q_ASSERT(m_winnerStrategy != nullptr);
    return m_winnerStrategy->snappedItems();
}

QPointF SnapManager::snappedPosition() const
{
    Q_ASSERT(m_winnerStrategy != nullptr);
    return m_winnerStrategy->snappedPosition();
}

SnapStrategy *SnapManager::snappingStrategy() const
{
    return m_winnerStrategy;
}
