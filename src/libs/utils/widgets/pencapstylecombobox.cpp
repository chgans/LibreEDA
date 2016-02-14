#include "pencapstylecombobox.h"

#include <QIcon>

static QMap<Qt::PenCapStyle, QString> styleToText = {
    { Qt::FlatCap, QString("Flat") },
    { Qt::SquareCap, QString("Square") },
    { Qt::RoundCap, QString("Round") }
};

static QIcon icon(Qt::PenCapStyle style)
{
    switch (style) {
    case Qt::FlatCap:   return QIcon::fromTheme("stroke-cap-butt");
    case Qt::SquareCap: return QIcon::fromTheme("stroke-cap-square");
    case Qt::RoundCap:  return QIcon::fromTheme("stroke-cap-round");
    default:            return QIcon();
    }
}

PenCapStyleComboBox::PenCapStyleComboBox(QWidget *parent) :
    QComboBox(parent)
{
    connect(this,
            static_cast<void (PenCapStyleComboBox::*)(int)>(&QComboBox::activated),
            [this](int index) {
        emit activated(itemData(index).value<Qt::PenCapStyle>());
    });
    connect(this,
            static_cast<void (PenCapStyleComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            [=](int index) {
        emit currentIndexChanged(itemData(index).value<Qt::PenCapStyle>());
    });
    connect(this,
            static_cast<void (PenCapStyleComboBox::*)(int)>(&QComboBox::highlighted),
            [=](int index) {
        emit highlighted(itemData(index).value<Qt::PenCapStyle>());
    });
}

void PenCapStyleComboBox::addItem(Qt::PenCapStyle style)
{
    QComboBox::addItem(icon(style),
                       styleToText[style],
                       QVariant::fromValue<Qt::PenCapStyle>(style));

}

void PenCapStyleComboBox::setCurrentIndex(Qt::PenCapStyle style)
{
    for (int index = 0; index < count(); index++) {
        if (itemData(index).value<Qt::PenCapStyle>() == style) {
            QComboBox::setCurrentIndex(index);
            return;
        }
    }
}
