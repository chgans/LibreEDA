#include "brushstylecombobox.h"

#include <QIcon>
#include <QPainter>

static QMap<Qt::BrushStyle, QString> styleToText = {
    { Qt::NoBrush, QString("None") },
    { Qt::Dense7Pattern, QString("Dots") },
    { Qt::HorPattern, QString("Horizontal") },
    { Qt::VerPattern, QString("Vertical") },
    { Qt::CrossPattern, QString("Cross") },
    { Qt::BDiagPattern, QString("Backward") },
    { Qt::FDiagPattern, QString("Forward") },
    { Qt::DiagCrossPattern, QString("Diagonal cross") }
};

static QIcon icon(Qt::BrushStyle style)
{
    QPixmap pixmap(16, 16);
    QPainter painter(&pixmap);
    painter.fillRect(pixmap.rect(), style);
    return QIcon(pixmap);
}

BrushStyleComboBox::BrushStyleComboBox(QWidget *parent):
    QComboBox(parent)
{
    connect(this,
            static_cast<void (BrushStyleComboBox::*)(int)>(&QComboBox::activated),
            [this](int index) {
        emit activated(itemData(index).value<Qt::BrushStyle>());
    });
    connect(this,
            static_cast<void (BrushStyleComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            [=](int index) {
        emit currentIndexChanged(itemData(index).value<Qt::BrushStyle>());
    });
    connect(this,
            static_cast<void (BrushStyleComboBox::*)(int)>(&QComboBox::highlighted),
            [=](int index) {
        emit highlighted(itemData(index).value<Qt::BrushStyle>());
    });
}

void BrushStyleComboBox::addItem(Qt::BrushStyle style)
{
    QComboBox::addItem(icon(style),
                       styleToText[style],
                       QVariant::fromValue<Qt::BrushStyle>(style));
}

void BrushStyleComboBox::setCurrentIndex(Qt::BrushStyle style)
{
    for (int index = 0; index < count(); index++) {
        if (itemData(index).value<Qt::BrushStyle>() == style) {
            QComboBox::setCurrentIndex(index);
            return;
        }
    }
}
