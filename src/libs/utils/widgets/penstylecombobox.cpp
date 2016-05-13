#include "penstylecombobox.h"

#include <QIcon>
#include <QPixmap>
#include <QPainter>

static QMap<Qt::PenStyle, QString> styleToText =
{
    { Qt::NoPen, QString("None") },
    { Qt::SolidLine, QString("Solid") },
    { Qt::DashLine, QString("Dash") },
    { Qt::DotLine, QString("Dot") },
    { Qt::DashDotLine, QString("Dash dot") },
    { Qt::DashDotDotLine, QString("Dash dot dot") },
    { Qt::CustomDashLine, QString("Custom") }
};

static QIcon icon(Qt::PenStyle style)
{
    QPixmap pix(64, 64);
    pix.fill(QColor(0, 0, 0, 0));

    QPen pen;
    pen.setColor(Qt::black);
    pen.setWidth(5);
    pen.setCapStyle(Qt::FlatCap);
    pen.setStyle(style);

    QPainter painter(&pix);
    painter.setPen(pen);
    painter.drawLine(1, 32, 62, 32);

    return QIcon(pix);
}

PenStyleComboBox::PenStyleComboBox(QWidget *parent) :
    QComboBox(parent)
{
    connect(this,
            static_cast<void (PenStyleComboBox::*)(int)>(&QComboBox::activated),
            [this](int index)
    {
        emit activated(itemData(index).value<Qt::PenStyle>());
    });
    connect(this,
            static_cast<void (PenStyleComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            [ = ](int index)
    {
        emit currentIndexChanged(itemData(index).value<Qt::PenStyle>());
    });
    connect(this,
            static_cast<void (PenStyleComboBox::*)(int)>(&QComboBox::highlighted),
            [ = ](int index)
    {
        emit highlighted(itemData(index).value<Qt::PenStyle>());
    });
}

void PenStyleComboBox::addItem(Qt::PenStyle style)
{
    QComboBox::addItem(icon(style), styleToText[style],
                       QVariant::fromValue<Qt::PenStyle>(style));
}

void PenStyleComboBox::setCurrentIndex(Qt::PenStyle style)
{
    for (int index = 0; index < count(); index++)
    {
        if (itemData(index).value<Qt::PenStyle>() == style)
        {
            QComboBox::setCurrentIndex(index);
            return;
        }
    }
}
