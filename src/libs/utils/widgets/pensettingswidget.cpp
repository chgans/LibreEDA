#include "pensettingswidget.h"
#include "penwidthcombobox.h"
#include "penstylecombobox.h"
#include "pencapstylecombobox.h"
#include "penjoinstylecombobox.h"
#include "pencolorcombobox.h"

#include <QFormLayout>
#include <QLabel>

PenSettingsWidget::PenSettingsWidget(QWidget *parent) :
    QWidget(parent)
{
    QFormLayout *layout = new QFormLayout();
    setLayout(layout);

    m_colorComboBox = new PenColorComboBox;
    m_colorComboBox->addItem("Primary", QColor("#839496"));
    m_colorComboBox->addItem("Secondary", QColor("#586e75"));
    m_colorComboBox->addItem("Emphasis", QColor("#93a1a1"));
    m_colorComboBox->addItem("Yellow", QColor("#b58900"));
    m_colorComboBox->addItem("Orange", QColor("#cb4b16"));
    m_colorComboBox->addItem("Red", QColor("#dc322f"));
    m_colorComboBox->addItem("Magenta", QColor("#d33682"));
    m_colorComboBox->addItem("Violet", QColor("#6c71c4"));
    m_colorComboBox->addItem("Blue", QColor("#268bd2"));
    m_colorComboBox->addItem("Cyan", QColor("#2aa198"));
    m_colorComboBox->addItem("Green", QColor("#859900"));
    m_colorComboBox->addItem("Background", QColor("#002b36"));
    m_colorComboBox->addItem("Highlight", QColor("#073642"));
    layout->addRow(new QLabel("Color"), m_colorComboBox);
    m_pen.setColor(m_colorComboBox->currentData().value<QColor>());
    connect(m_colorComboBox, &PenColorComboBox::currentIndexChanged,
            this, [this](QColor color) {
        if (m_pen.color() == color)
            return;
        m_pen.setColor(color);
        emit penChanged(m_pen);
    });

    // http://www.metrication.com/drafting/lines.html
    m_widthComboBox = new PenWidthComboBox;
    m_widthComboBox->addItem("Cosmetic", 0.0);
    m_widthComboBox->addItem("Thin", 0.18);
    m_widthComboBox->addItem("Medium", 0.25);
    m_widthComboBox->addItem("Thick", 0.35);
    layout->addRow(new QLabel("Width"), m_widthComboBox);
    m_pen.setWidth(m_widthComboBox->currentData().value<qreal>());
    connect(m_widthComboBox, &PenWidthComboBox::currentIndexChanged,
            this, [this](qreal width) {
        if (qFuzzyCompare(m_pen.widthF(), width))
            return;
        m_pen.setWidthF(width);
        emit penChanged(m_pen);
    });

    m_styleComboBox = new PenStyleComboBox;
    m_styleComboBox->addItem(Qt::SolidLine);
    m_styleComboBox->addItem(Qt::DashLine);
    m_styleComboBox->addItem(Qt::DotLine);
    m_styleComboBox->addItem(Qt::DashDotLine);
    m_styleComboBox->addItem(Qt::DashDotDotLine);
    layout->addRow(new QLabel("Style"), m_styleComboBox);
    m_pen.setStyle(m_styleComboBox->currentData().value<Qt::PenStyle>());
    connect(m_styleComboBox, &PenStyleComboBox::currentIndexChanged,
            this, [this](Qt::PenStyle style) {
        if (m_pen.style() == style)
            return;
        m_pen.setStyle(style);
        emit penChanged(m_pen);
    });

    m_capStyleComboBox = new PenCapStyleComboBox;
    m_capStyleComboBox->addItem(Qt::FlatCap);
    m_capStyleComboBox->addItem(Qt::SquareCap);
    m_capStyleComboBox->addItem(Qt::RoundCap);
    layout->addRow(new QLabel("Cap"), m_capStyleComboBox);
    m_pen.setCapStyle(m_capStyleComboBox->currentData().value<Qt::PenCapStyle>());
    connect(m_capStyleComboBox, &PenCapStyleComboBox::currentIndexChanged,
            this, [this](Qt::PenCapStyle style) {
        if (m_pen.capStyle() == style)
            return;
        m_pen.setCapStyle(style);
        emit penChanged(m_pen);
    });

    m_joinStyleComboBox = new PenJoinStyleComboBox;
    m_joinStyleComboBox->addItem(Qt::BevelJoin);
    m_joinStyleComboBox->addItem(Qt::MiterJoin);
    m_joinStyleComboBox->addItem(Qt::RoundJoin);
    layout->addRow(new QLabel("Join"), m_joinStyleComboBox);
    m_pen.setJoinStyle(m_joinStyleComboBox->currentData().value<Qt::PenJoinStyle>());
    connect(m_joinStyleComboBox, &PenJoinStyleComboBox::currentIndexChanged,
            this, [this](Qt::PenJoinStyle style) {
        if (m_pen.joinStyle() == style)
            return;
        m_pen.setJoinStyle(style);
        emit penChanged(m_pen);
    });
}

QPen PenSettingsWidget::pen() const
{
    return m_pen;
}

void PenSettingsWidget::setPen(const QPen &pen)
{
    if (m_pen != pen) {
        m_pen = pen;
        m_colorComboBox->setCurrentIndex(m_pen.color());
        m_widthComboBox->setCurrentIndex(m_pen.widthF());
        m_styleComboBox->setCurrentIndex(m_pen.style());
        m_capStyleComboBox->setCurrentIndex(m_pen.capStyle());
        m_joinStyleComboBox->setCurrentIndex(m_pen.joinStyle());
    }
}
