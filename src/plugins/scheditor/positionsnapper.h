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

// End point
// Mid point
// Entity point
// Center
// Intersection


class SnapManager: public QObject
{
    Q_OBJECT

public:
    SnapManager(SchView *view);

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
};

#endif // POSITIONSNAPPER_H
