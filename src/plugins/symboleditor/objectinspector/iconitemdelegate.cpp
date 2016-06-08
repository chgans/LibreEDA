#include "iconitemdelegate.h"

#include <QPainter>
#include <QEvent>

namespace SymbolEditor
{

    IconItemDelegate::IconItemDelegate(QObject *parent):
        QAbstractItemDelegate (parent),
        m_iconSize(16, 16)
    {

    }

    void IconItemDelegate::setIconSize(const QSize &size)
    {
        m_iconSize = size;
    }

    QSize IconItemDelegate::iconSize() const
    {
        return m_iconSize;
    }

    void IconItemDelegate::setCheckedIcon(const QIcon &icon)
    {
        m_checkedIcon = icon;
    }

    QIcon IconItemDelegate::checkedIcon() const
    {
        return m_checkedIcon;
    }

    void IconItemDelegate::setUncheckedIcon(const QIcon &icon)
    {
        m_uncheckedIcon = icon;
    }

    QIcon IconItemDelegate::uncheckedIcon() const
    {
        return m_uncheckedIcon;
    }


    void IconItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        if (option.state & QStyle::State_Selected)
        {
            painter->fillRect(option.rect, option.palette.highlight());
        }

        painter->save();
        bool checked = index.model()->data(index, Qt::DisplayRole).toBool();
        if (checked)
        {
            m_checkedIcon.paint(painter, option.rect);
        }
        else
        {
            m_uncheckedIcon.paint(painter, option.rect);
        }
        painter->restore();
    }

    QSize IconItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        Q_UNUSED(option);
        Q_UNUSED(index);

        return m_iconSize;
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
