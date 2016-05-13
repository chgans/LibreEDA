#include "pencolorcombobox.h"

#include <QIcon>
#include <QPixmap>

static QIcon icon(QColor color)
{
    QPixmap pixmap(16, 16);
    pixmap.fill(color);
    return QIcon(pixmap);
}

PenColorComboBox::PenColorComboBox(QWidget *parent):
    QComboBox(parent)
{
    connect(this,
            static_cast<void (PenColorComboBox::*)(int)>(&QComboBox::activated),
            [this](int index)
    {
        emit activated(itemData(index).value<QColor>());
    });
    connect(this,
            static_cast<void (PenColorComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            [ = ](int index)
    {
        emit currentIndexChanged(itemData(index).value<QColor>());
    });
    connect(this,
            static_cast<void (PenColorComboBox::*)(int)>(&QComboBox::highlighted),
            [ = ](int index)
    {
        emit highlighted(itemData(index).value<QColor>());
    });

}

void PenColorComboBox::addItem(const QString &label, QColor color)
{
    QComboBox::addItem(icon(color),
                       label,
                       QVariant::fromValue<QColor>(color));
}

void PenColorComboBox::setCurrentIndex(QColor color)
{
    for (int index = 0; index < count(); index++)
    {
        if (itemData(index).value<QColor>() == color)
        {
            QComboBox::setCurrentIndex(index);
            return;
        }
    }
}
