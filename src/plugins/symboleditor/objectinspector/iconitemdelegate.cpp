#include "iconitemdelegate.h"

#include <QPainter>
#include <QEvent>

namespace SymbolEditor
{

    IconItemDelegate::IconItemDelegate(QObject *parent):
        QAbstractItemDelegate (parent),
        m_size(16, 16)
    {

    }

    QSize IconItemDelegate::iconSize() const
    {
        return m_size;
    }

    void IconItemDelegate::setIconSize(const QSize &size)
    {
        if (m_size == size)
        {
            return;
        }

        m_size = size;
    }


    void IconItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        if (option.state & QStyle::State_Selected)
        {
            painter->fillRect(option.rect, option.palette.highlight());
        }

        bool active = index.model()->data(index, Qt::DisplayRole).toBool();

        QString name;
        if (active)
        {
            name = activeIconName;
        }
        else
        {
            name = inactiveIconName;
        }

        QIcon icon = QIcon::fromTheme(name);
        painter->save();
        icon.paint(painter, option.rect);
        painter->restore();
    }

    QSize IconItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        Q_UNUSED(option);
        Q_UNUSED(index);

        return m_size;
    }

    bool IconItemDelegate::editorEvent(QEvent *event, QAbstractItemModel *model,
                                       const QStyleOptionViewItem &option, const QModelIndex &index)
    {
        Q_UNUSED(option);

        if (event->type() != QEvent::MouseButtonRelease)
        {
            return false;
        }

        bool active = model->data(index, Qt::DisplayRole).toBool();
        model->setData(index, !active, Qt::EditRole);
        return true;
    }

}
