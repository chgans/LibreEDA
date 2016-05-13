#include "brushsettingswidget.h"
#include "brushstylecombobox.h"
#include "pencolorcombobox.h"

#include <QFormLayout>
#include <QLabel>

BrushSettingsWidget::BrushSettingsWidget(QWidget *parent):
    QWidget(parent)
{
    QFormLayout *layout = new QFormLayout();
    setLayout(layout);

    m_colorComboBox = new PenColorComboBox;
    m_colorComboBox->addItem("Background", QColor("#002b36"));
    m_colorComboBox->addItem("Highlight", QColor("#073642"));
    m_colorComboBox->addItem("Yellow", QColor("#b58900"));
    m_colorComboBox->addItem("Orange", QColor("#cb4b16"));
    m_colorComboBox->addItem("Red", QColor("#dc322f"));
    m_colorComboBox->addItem("Magenta", QColor("#d33682"));
    m_colorComboBox->addItem("Violet", QColor("#6c71c4"));
    m_colorComboBox->addItem("Blue", QColor("#268bd2"));
    m_colorComboBox->addItem("Cyan", QColor("#2aa198"));
    m_colorComboBox->addItem("Green", QColor("#859900"));
    m_colorComboBox->addItem("Primary", QColor("#839496"));
    m_colorComboBox->addItem("Secondary", QColor("#586e75"));
    m_colorComboBox->addItem("Emphasis", QColor("#93a1a1"));
    layout->addRow(new QLabel("Color"), m_colorComboBox);
    m_brush.setColor(m_colorComboBox->currentData().value<QColor>());
    connect(m_colorComboBox, &PenColorComboBox::currentIndexChanged,
            this, [this](QColor color)
    {
        if (m_brush.color() == color)
        {
            return;
        }
        m_brush.setColor(color);
        emit brushChanged(m_brush);
    });

    m_styleComboBox = new BrushStyleComboBox;
    m_styleComboBox->addItem(Qt::SolidPattern);
    m_styleComboBox->addItem(Qt::Dense7Pattern);
    m_styleComboBox->addItem(Qt::CrossPattern);
    m_styleComboBox->addItem(Qt::DiagCrossPattern);
    m_styleComboBox->addItem(Qt::NoBrush);
    layout->addRow(new QLabel("Style"), m_styleComboBox);
    m_brush.setStyle(m_styleComboBox->currentData().value<Qt::BrushStyle>());
    connect(m_styleComboBox, &BrushStyleComboBox::currentIndexChanged,
            this, [this](Qt::BrushStyle style)
    {
        if (m_brush.style() == style)
        {
            return;
        }
        m_brush.setStyle(style);
        emit brushChanged(m_brush);
    });

}

QBrush BrushSettingsWidget::brush() const
{
    return m_brush;
}

void BrushSettingsWidget::setBrush(const QBrush &brush)
{
    if (m_brush == brush)
    {
        return;
    }
    m_brush = brush;
    emit brushChanged(m_brush);
}
