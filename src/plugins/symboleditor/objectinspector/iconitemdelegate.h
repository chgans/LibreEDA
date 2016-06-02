#pragma once

#include <QAbstractItemDelegate>

namespace SymbolEditor
{

    class IconItemDelegate : public QAbstractItemDelegate
    {
    public:
        IconItemDelegate(QObject *parent = nullptr);

        QSize iconSize() const;

        QString activeIconName;
        QString inactiveIconName;

    public slots:
        void setIconSize(const QSize &size);

    private:
        QSize m_size;

        // QAbstractItemDelegate interface
    public:
        void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
        QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

        // QAbstractItemDelegate interface
    public:
        bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);
    };

}
