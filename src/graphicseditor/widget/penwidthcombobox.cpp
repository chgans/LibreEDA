#include "penwidthcombobox.h"

#include <QIcon>
#include <QPixmap>
#include <QPainter>

static QIcon icon(qreal width)
{
    QPixmap pix(64, 64);
    pix.fill(QColor(0, 0, 0, 0));

    QPen pen;
    pen.setColor(Qt::black);
    pen.setWidth(width);
    pen.setCapStyle(Qt::FlatCap);
    pen.setStyle(Qt::SolidLine);

    QPainter painter(&pix);
    painter.setPen(pen);
    painter.drawLine(1, 32, 62, 32);

    return QIcon(pix);
}

PenWidthComboBox::PenWidthComboBox(QWidget *parent) :
    QComboBox(parent)
{
    connect(this,
            static_cast<void (PenWidthComboBox::*)(int)>(&QComboBox::activated),
            [this](int index) {
        emit activated(itemData(index).value<qreal>());
    });
    connect(this,
            static_cast<void (PenWidthComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            [=](int index) {
        emit currentIndexChanged(itemData(index).value<qreal>());
    });
    connect(this,
            static_cast<void (PenWidthComboBox::*)(int)>(&QComboBox::highlighted),
            [=](int index) {
        emit highlighted(itemData(index).value<qreal>());
    });
}

void PenWidthComboBox::addItem(const QString &label, qreal width)
{
    QComboBox::addItem(icon(width), label, QVariant::fromValue<qreal>(width));
}

void PenWidthComboBox::setCurrentIndex(qreal width)
{
    for (int index = 0; index < count(); index++) {
        if (itemData(index).value<qreal>() == width) {
            QComboBox::setCurrentIndex(index);
            return;
        }
    }
}
