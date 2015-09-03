#include "positionsnapper.h"
#include "schview.h"
#include "schscene.h"
#include "schitem.h"
#include "graphicsgrid.h"

#include <QAction>


SnapStrategy::SnapStrategy(SchView *view):
    m_view(view),
    m_action(new QAction(nullptr))
{

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

void SnapStrategy::updateAction()
{
    m_action->setShortcut(m_shortcut);
    m_action->setToolTip(QString("%1 <i>%2</i>").arg(m_label).arg(m_shortcut.toString()));
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

SnapToGridStrategy::SnapToGridStrategy(SchView *view):
    SnapStrategy(view)
{
    setLabel("Snap to grid");
    setName("Grid");
    setShortcut(QKeySequence("S,G"));
    setIcon(QIcon(":/icons/graphicssnapgrid.svg"));
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

SnapToItemHotSpotsStrategy::SnapToItemHotSpotsStrategy(SchView *view):
    SnapStrategy(view)
{
    setLabel("Snap to reference");
    setName("Reference");
    setShortcut(QKeySequence("S,R"));
    setIcon(QIcon(":/icons/graphicssnapobject.svg"));
    updateAction();
}

bool SnapToItemHotSpotsStrategy::snap(QPoint mousePos, qreal maxDistance)
{
    QPainterPath region;
    region.addEllipse(mousePos, maxDistance, maxDistance);

    // Snap points candidates
    QMultiMap<SchItem*, QPoint> candidates;

    // Snap to hot spots
    QList<QGraphicsItem*> items = view()->items(region, Qt::IntersectsItemShape);
    foreach (QGraphicsItem *i, items) {
        SchItem *item = dynamic_cast<SchItem*>(i); // TODO: use item.type()
        if (item == nullptr)
            continue;
        foreach (QPointF hotSpot, item->hotSpots()) {
            QPoint pos = view()->mapFromScene(item->mapToScene(hotSpot));
            candidates.insert(item, pos);
        }
    }

    if (candidates.isEmpty())
        return false;

    // Pick the closest candidate
    QPoint snapPos = mousePos;
    SchItem *snapItem;
    int min = INT_MAX;
    foreach (SchItem *item, candidates.keys()) {
        foreach (QPoint pos, candidates.values(item)) {
            int length = (pos - mousePos).manhattanLength();
            if (length < min) {
                snapItem = item;
                snapPos = pos;
                min = length;
            }
        }
    }
    setSnappedItems(QList<SchItem*>() << snapItem);
    setSnappedPosition(snapPos);
    return true;
}

SnapManager::SnapManager(SchView *view):
    QObject(view)
{
    m_strategies << new SnapToGridStrategy(view)
                 << new SnapToItemHotSpotsStrategy(view);
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
        if (group == "all" || strategy->group() == group)
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
    int distance = INT_MAX;
    foreach (SnapStrategy *strategy, m_strategies) {
        if (strategy->snap(mousePos, maxDistance)) {
            if ((strategy->snappedPosition() - mousePos).manhattanLength() < distance)
                m_winnerStrategy = strategy;
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
