#ifndef POSITIONSNAPPER_H
#define POSITIONSNAPPER_H

#include <QPoint>
#include <QPainterPath>
#include <QIcon>
#include <QKeySequence>

class SchView;
class SchItem;
class QAction;

class SnapStrategy
{
public:
    SnapStrategy(SchView *view);
    virtual ~SnapStrategy();

    QString label() const;
    QString name() const;
    QKeySequence shortcut() const;
    QString group() const;
    QAction *action() const;
    QIcon icon() const;

    virtual QPainterPath decoration() const;
    virtual bool snap(QPoint mousePos, qreal maxDistance);

    QList<SchItem *> snappedItems() const;
    QPoint snappedPosition() const;

    bool isEnabled() const;

protected:
    SchView *view();

    void setGroup(const QString &name);
    void setLabel(const QString &label);
    void setName(const QString &name);
    void setShortcut(const QKeySequence &shortcut);
    void setIcon(const QIcon &icon);
    void updateAction();

    void setSnappedPosition(QPoint pos);
    void setSnappedItems(const QList<SchItem*> &items);

    QList<SchItem *> itemsNearby(QPoint pos, qreal maxDistance);
    QPair<SchItem *, QPoint> closestItemPoint(QPoint pos, const QMultiMap<SchItem *, QPoint> &candidates);

private:
    SchView *m_view;

    QAction *m_action;
    QString m_group;
    QString m_label;
    QString m_name;
    QIcon m_icon;
    QKeySequence m_shortcut;

    QPoint m_snappedPosition;
    QList<SchItem*> m_snappedItems;
};

class NoSnapStrategy: public SnapStrategy
{
public:
    NoSnapStrategy(SchView *view);

    bool snap(QPoint mousePos, qreal maxDistance);
};

class SnapToGridStrategy: public SnapStrategy
{
public:
    SnapToGridStrategy(SchView *view);

    bool snap(QPoint mousePos, qreal maxDistance);
};

class SnapToItemHotSpotsStrategy:  public SnapStrategy
{
public:
    SnapToItemHotSpotsStrategy(SchView *view);

    bool snap(QPoint mousePos, qreal maxDistance);
};

class SnapToItemEndPointStrategy: public SnapStrategy
{
public:
    SnapToItemEndPointStrategy(SchView *view);

    bool snap(QPoint mousePos, qreal maxDistance);
};

class SnapToItemMidPointStrategy: public SnapStrategy
{
public:
    SnapToItemMidPointStrategy(SchView *view);

    bool snap(QPoint mousePos, qreal maxDistance);
};

// Entity point
class SnapToItemShapeStrategy: public SnapStrategy
{
public:
    SnapToItemShapeStrategy(SchView *view);

    bool snap(QPoint mousePos, qreal maxDistance);

};

// Center
class SnapToItemCenterStrategy: public SnapStrategy
{
public:
    SnapToItemCenterStrategy(SchView *view);

    bool snap(QPoint mousePos, qreal maxDistance);

};

// Intersection


class SnapManager: public QObject
{
    Q_OBJECT

public:
    SnapManager(SchView *view);
    ~SnapManager();

    QList<QString> groups() const;
    QList<QAction *> actions(const QString &group = QString("all")) const;

    QPainterPath decoration() const;
    bool snap(QPoint mousePos, qreal maxDistance);
    QList<SchItem *> snappedItems() const;
    QPoint snappedPosition() const;
    SnapStrategy * snappingStrategy() const;

private:
    QList<QString> m_groups;
    QList<QAction *> m_actions;
    QList<SnapStrategy *> m_strategies;
    SnapStrategy *m_winnerStrategy;
    SnapStrategy *m_defaultStrategy;
};

#endif // POSITIONSNAPPER_H
