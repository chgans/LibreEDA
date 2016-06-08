#pragma once

#include <QAbstractItemDelegate>

namespace SymbolEditor
{

    class IconItemDelegate : public QAbstractItemDelegate
    {
    public:
        IconItemDelegate(QObject *parent = nullptr);

        QSize iconSize() const;
        QIcon checkedIcon() const;
        QIcon uncheckedIcon() const;

    public slots:
        void setIconSize(const QSize &size);
        void setCheckedIcon(const QIcon &icon);
        void setUncheckedIcon(const QIcon &icon);


    private:
        QSize m_iconSize;
        QIcon m_checkedIcon;
        QIcon m_uncheckedIcon;

        // QAbstractItemDelegate interface
    public:
        void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
        QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
        bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);
    };

}
