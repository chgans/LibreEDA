#pragma once

#include <QPointF>
#include <QPainterPath>
#include <QIcon>
#include <QKeySequence>

class QAction;

namespace SymbolEditor
{

    class View;
    class Item;

    class SnapStrategy
    {
    public:
        explicit SnapStrategy(View *view);
        virtual ~SnapStrategy();

        QString label() const;
        QString name() const;
        QKeySequence shortcut() const;
        QString group() const;
        QAction *action() const;
        QIcon icon() const;

        virtual QPainterPath decoration() const;
        virtual bool snap(QPointF mousePos, qreal maxDistance);

        QList<Item *> snappedItems() const;
        QPointF snappedPosition() const;

        bool isEnabled() const;

    protected:
        View *view();

        void setGroup(const QString &name);
        void setLabel(const QString &label);
        void setName(const QString &name);
        void setShortcut(const QKeySequence &shortcut);
        void setIcon(const QIcon &icon);
        void updateAction();

        void setSnappedPosition(QPointF pos);
        void setSnappedItems(const QList<Item *> &items);

        QList<Item *> itemsNearby(QPointF pos, qreal maxDistance);
        QPair<Item *, QPointF> closestItemPoint(QPointF pos,
                                                const QMultiMap<Item *, QPointF> &candidates);

    private:
        View *m_view;

        QAction *m_action;
        QString m_group;
        QString m_label;
        QString m_name;
        QIcon m_icon;
        QKeySequence m_shortcut;

        QPointF m_snappedPosition;
        QList<Item *> m_snappedItems;
    };

    class NoSnapStrategy: public SnapStrategy
    {
    public:
        explicit NoSnapStrategy(View *view);

        bool snap(QPointF mousePos, qreal maxDistance);
    };

    class SnapToGridStrategy: public SnapStrategy
    {
    public:
        explicit SnapToGridStrategy(View *view);

        bool snap(QPointF mousePos, qreal maxDistance);
    };

    class SnapToItemHotSpotsStrategy:  public SnapStrategy
    {
    public:
        explicit SnapToItemHotSpotsStrategy(View *view);

        bool snap(QPointF mousePos, qreal maxDistance);
    };

    class SnapToItemEndPointStrategy: public SnapStrategy
    {
    public:
        explicit SnapToItemEndPointStrategy(View *view);

        bool snap(QPointF mousePos, qreal maxDistance);
    };

    class SnapToItemMidPointStrategy: public SnapStrategy
    {
    public:
        explicit SnapToItemMidPointStrategy(View *view);

        bool snap(QPointF mousePos, qreal maxDistance);
    };

    // Entity point
    class SnapToItemShapeStrategy: public SnapStrategy
    {
    public:
        explicit SnapToItemShapeStrategy(View *view);

        bool snap(QPointF mousePos, qreal maxDistance);

    };

    // Center
    class SnapToItemCenterStrategy: public SnapStrategy
    {
    public:
        explicit SnapToItemCenterStrategy(View *view);

        bool snap(QPointF mousePos, qreal maxDistance);

    };

    // Intersection


    class SnapManager: public QObject
    {
        Q_OBJECT

    public:
        explicit SnapManager(View *view);
        ~SnapManager();

        QList<QString> groups() const;
        QList<QAction *> actions(const QString &group = QString("all")) const;

        QPainterPath decoration() const;
        bool snap(QPointF mousePos, qreal maxDistance);
        QList<Item *> snappedItems() const;
        QPointF snappedPosition() const;
        SnapStrategy *snappingStrategy() const;

    private:
        QList<QString> m_groups;
        QList<QAction *> m_actions;
        QList<SnapStrategy *> m_strategies;
        SnapStrategy *m_winnerStrategy;
        SnapStrategy *m_defaultStrategy;
    };

}
