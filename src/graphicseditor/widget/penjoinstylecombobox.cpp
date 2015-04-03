#include "penjoinstylecombobox.h"

#include <QIcon>

static QMap<Qt::PenJoinStyle, QString> styleToText = {
    { Qt::BevelJoin, QString("Bevel") },
    { Qt::MiterJoin, QString("Mitter") },
    { Qt::RoundJoin, QString("Round") }
};

static QIcon icon(Qt::PenJoinStyle style)
{
    switch (style) {
    case Qt::BevelJoin:   return QIcon::fromTheme("stroke-join-bevel");
    case Qt::MiterJoin: return QIcon::fromTheme("stroke-join-miter");
    case Qt::RoundJoin:  return QIcon::fromTheme("stroke-join-round");
    default:            return QIcon();
    }
}

PenJoinStyleComboBox::PenJoinStyleComboBox(QWidget *parent) :
    QComboBox(parent)
{
    connect(this,
            static_cast<void (PenJoinStyleComboBox::*)(int)>(&QComboBox::activated),
            [this](int index) {
        emit activated(itemData(index).value<Qt::PenJoinStyle>());
    });
    connect(this,
            static_cast<void (PenJoinStyleComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            [=](int index) {
        emit currentIndexChanged(itemData(index).value<Qt::PenJoinStyle>());
    });
    connect(this,
            static_cast<void (PenJoinStyleComboBox::*)(int)>(&QComboBox::highlighted),
            [=](int index) {
        emit highlighted(itemData(index).value<Qt::PenJoinStyle>());
    });
}

void PenJoinStyleComboBox::addItem(Qt::PenJoinStyle style)
{
    QComboBox::addItem(icon(style),
                       styleToText[style],
                       QVariant::fromValue<Qt::PenJoinStyle>(style));
}

void PenJoinStyleComboBox::setCurrentIndex(Qt::PenJoinStyle style)
{
    for (int index = 0; index < count(); index++) {
        if (itemData(index).value<Qt::PenJoinStyle>() == style) {
            QComboBox::setCurrentIndex(index);
            return;
        }
    }
}
