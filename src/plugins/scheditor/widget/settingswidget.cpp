#include "core/core.h"
#include "settingswidget.h"
#include "ui_settingswidget.h"
#include "schscene.h"
#include "grid/graphicscartesiangrid.h"
#include "palette.h"

#include <QScrollBar>
#include <QOpenGLWidget>
#include <QSettings>

SettingsWidget::SettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsWidget)
{
    ui->setupUi(this);

    auto scene = new SchScene(this);
    scene->setSceneRect(-100, -100, 200, 200);
    ui->graphicsView->setScene(scene);


    ui->colorSchemeComboBox->addItem("Dark", QVariant::fromValue<Palette::Mode>(Palette::Dark));
    m_colorSchemeToIndex.insert(Palette::Dark, 0);
    ui->colorSchemeComboBox->addItem("Light", QVariant::fromValue<Palette::Mode>(Palette::Light));
    m_colorSchemeToIndex.insert(Palette::Light, 1);
    ui->colorSchemeComboBox->setCurrentIndex(m_colorSchemeToIndex[ui->graphicsView->paletteMode()]);
    connect(ui->colorSchemeComboBox, &QComboBox::currentTextChanged,
            this, [this](const QString &)
    {
        ui->graphicsView->setPaletteMode(ui->colorSchemeComboBox->currentData().value<Palette::Mode>());
    });

    ui->rulerCheckBox->setChecked(ui->graphicsView->rulerEnabled());
    connect(ui->rulerCheckBox, &QCheckBox::toggled,
            this, [this](bool checked)
    {
       ui->graphicsView->setRulerEnabled(checked);
    });

    ui->gridCheckBox->setChecked(ui->graphicsView->gridEnabled());
    ui->gridGroupBox->setEnabled(ui->graphicsView->gridEnabled());
    connect(ui->gridCheckBox, &QCheckBox::toggled,
            this, [this](bool checked)
    {
        ui->gridGroupBox->setEnabled(checked);
        ui->graphicsView->setGridEnabled(checked);
    });

    updateGuiScrollBars();
    connect(ui->scrollBarsCheckBox, &QCheckBox::toggled,
            this, [this](bool)
    {
        updateViewScrollBars();
        updateGuiScrollBars();
    });
    connect(ui->scrollBarsCheckBox, &QCheckBox::toggled,
            this, [this](bool)
    {
        updateViewScrollBars();
        updateGuiScrollBars();
    });

    updateGuiMouseCursor();
    connect(ui->cusorCrosshairCheckBox, &QCheckBox::toggled,
            this, [this](bool)
    {
        updateViewMouseCursor();
        updateGuiMouseCursor();
    });
    connect(ui->largeCursorCrosshairCheckBox, &QCheckBox::toggled,
            this, [this](bool)
    {
        updateViewMouseCursor();
        updateGuiMouseCursor();
    });

    updateGuiOriginMark();
    connect(ui->originCheckBox, &QCheckBox::toggled,
            this, [this](bool)
    {
        updateViewOriginMark();
        updateGuiOriginMark();
    });
    connect(ui->largeOriginAxisCheckBox, &QCheckBox::toggled,
            this, [this](bool)
    {
        updateViewOriginMark();
        updateGuiOriginMark();
    });

    ui->minimalGridSizeSpinBox->setValue(ui->graphicsView->minimalGridSize());
    connect(ui->minimalGridSizeSpinBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, [this](int value)
    {
        ui->graphicsView->setMinimalGridSize(value);
    });

    ui->coarseMultiplierSpinBox->setValue(ui->graphicsView->gridCoarseMultiplier());
    connect(ui->coarseMultiplierSpinBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, [this](int value)
    {
        ui->graphicsView->setGridCoarseMultiplier(value);
    });

    ui->solidCoarseGridLinesCheckBox->setChecked(ui->graphicsView->gridCoarseLineStyle() == Qt::SolidLine);
    connect(ui->solidCoarseGridLinesCheckBox, &QCheckBox::toggled,
            this, [this](bool checked)
    {
       if (checked)
       {
           ui->graphicsView->setGridCoarseLineStyle(Qt::SolidLine);
       }
       else
       {
           ui->graphicsView->setGridCoarseLineStyle(Qt::DotLine);
       }
    });

    ui->solidFineGridLinesCheckBox->setChecked(ui->graphicsView->gridFineLineStyle() == Qt::SolidLine);
    connect(ui->solidFineGridLinesCheckBox, &QCheckBox::toggled,
            this, [this](bool checked)
    {
       if (checked)
       {
           ui->graphicsView->setGridFineLineStyle(Qt::SolidLine);
       }
       else
       {
           ui->graphicsView->setGridFineLineStyle(Qt::DotLine);
       }
    });

    connect(ui->antiAliasingCheckBox, &QCheckBox::toggled,
            this, [this](bool checked)
    {
        ui->graphicsView->setRenderHint(QPainter::Antialiasing, checked);
        ui->graphicsView->setRenderHint(QPainter::TextAntialiasing, checked);
        ui->graphicsView->setRenderHint(QPainter::HighQualityAntialiasing, checked);
    });

    ui->openGlCheckBox->setChecked(ui->graphicsView->hardwareAccelerationEnabled());
    connect(ui->openGlCheckBox, &QCheckBox::toggled,
            this, [this](bool checked)
    {
        ui->graphicsView->setHardwareAccelerationEnabled(checked);
    });
}

SettingsWidget::~SettingsWidget()
{
    delete ui;
}

void SettingsWidget::loadSettings()
{
    QSettings *settings = Core::settings();
    settings->beginGroup("SchEditor/Appearance");

    Palette::Mode mode = Palette::Dark;
    QString paletteModeString = settings->value("PaletteMode", QString("Dark")).toString();
    if (paletteModeString == "Light")
    {
        mode = Palette::Light;
    }
    ui->colorSchemeComboBox->setCurrentIndex(m_colorSchemeToIndex.value(mode));
    ui->rulerCheckBox->setChecked(settings->value("RulerEnabled", true).toBool());
    ui->gridCheckBox->setChecked(settings->value("GridEnabled", true).toBool());

    settings->endGroup();
    settings->sync();
}

void SettingsWidget::saveSettings()
{
    QSettings *settings = Core::settings();
    settings->beginGroup("SchEditor/Appearance");

    settings->setValue("PaletteMode", ui->colorSchemeComboBox->currentData().value<Palette::Mode>());
    settings->setValue("RulerEnabled", ui->rulerCheckBox->isChecked());
    settings->setValue("GridEnabled", ui->gridCheckBox->isChecked());

    settings->endGroup();
}

void SettingsWidget::updateViewScrollBars()
{
    if (!ui->scrollBarsCheckBox->isChecked())
    {
        ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }
    else if (ui->scrollBarsAsNeededCheckBox->isChecked())
    {
        ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    }
    else
    {
        ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    }
}

void SettingsWidget::updateGuiScrollBars()
{
    switch (ui->graphicsView->horizontalScrollBarPolicy()) {
    case Qt::ScrollBarAlwaysOff:
        ui->scrollBarsCheckBox->setChecked(false);
        ui->scrollBarsAsNeededCheckBox->setChecked(false);
        break;
    case Qt::ScrollBarAsNeeded:
        ui->scrollBarsCheckBox->setChecked(true);
        ui->scrollBarsAsNeededCheckBox->setChecked(true);
        break;
    case Qt::ScrollBarAlwaysOn:
        ui->scrollBarsCheckBox->setChecked(true);
        ui->scrollBarsAsNeededCheckBox->setChecked(false);
        break;
    }
    ui->scrollBarsAsNeededCheckBox->setEnabled(ui->scrollBarsCheckBox->isChecked());
}

void SettingsWidget::updateViewMouseCursor()
{

    if (!ui->cusorCrosshairCheckBox->isChecked())
    {
        ui->graphicsView->setMouseCursor(SchView::NoMouseCursor);
    }
    else if (ui->largeCursorCrosshairCheckBox->isChecked())
    {
        ui->graphicsView->setMouseCursor(SchView::LargeMouseCursor);
    }
    else
        ui->graphicsView->setMouseCursor(SchView::SmallMouseCursor);
}

void SettingsWidget::updateGuiMouseCursor()
{
    switch (ui->graphicsView->mouseCursor()) {
    case SchView::NoMouseCursor:
        ui->cusorCrosshairCheckBox->setChecked(false);
        ui->largeCursorCrosshairCheckBox->setChecked(false);
        break;
    case SchView::SmallMouseCursor:
        ui->cusorCrosshairCheckBox->setChecked(true);
        ui->largeCursorCrosshairCheckBox->setChecked(false);
        break;
    case SchView::LargeMouseCursor:
        ui->cusorCrosshairCheckBox->setChecked(true);
        ui->largeCursorCrosshairCheckBox->setChecked(true);
    }
    ui->largeCursorCrosshairCheckBox->setEnabled(ui->cusorCrosshairCheckBox->isChecked());
}

void SettingsWidget::updateViewOriginMark()
{
    if (!ui->originCheckBox->isChecked())
    {
        ui->graphicsView->setOriginMark(SchView::NoOriginMark);
    }
    else if (ui->largeOriginAxisCheckBox->isChecked())
    {
        ui->graphicsView->setOriginMark(SchView::LargeOriginMark);
    }
    else
        ui->graphicsView->setOriginMark(SchView::SmallOriginMark);
}

void SettingsWidget::updateGuiOriginMark()
{
    switch (ui->graphicsView->originMark()) {
    case SchView::NoOriginMark:
        ui->originCheckBox->setChecked(false);
        ui->largeOriginAxisCheckBox->setChecked(false);
        break;
    case SchView::SmallOriginMark:
        ui->originCheckBox->setChecked(true);
        ui->largeOriginAxisCheckBox->setChecked(false);
        break;
    case SchView::LargeOriginMark:
        ui->originCheckBox->setChecked(true);
        ui->largeOriginAxisCheckBox->setChecked(true);
    }
    ui->largeOriginAxisCheckBox->setEnabled(ui->originCheckBox->isChecked());
}
