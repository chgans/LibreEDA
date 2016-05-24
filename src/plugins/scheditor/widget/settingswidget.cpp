#include "core/core.h"
#include "settingswidget.h"
#include "ui_settingswidget.h"
#include "schscene.h"
#include "grid/graphicscartesiangrid.h"
#include "palette.h"
#include "scheditorsettings.h"

#include <QScrollBar>
#include <QOpenGLWidget>
#include <QSettings>

SettingsWidget::SettingsWidget(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::SettingsWidget)
{
    m_ui->setupUi(this);

    auto scene = new Scene(this);
    scene->setSceneRect(-100, -100, 200, 200);
    m_ui->graphicsView->setScene(scene);


    m_ui->colorSchemeComboBox->addItem("Dark", QVariant::fromValue<Palette::Mode>(Palette::Dark));
    m_colorSchemeToIndex.insert(Palette::Dark, 0);
    m_ui->colorSchemeComboBox->addItem("Light", QVariant::fromValue<Palette::Mode>(Palette::Light));
    m_colorSchemeToIndex.insert(Palette::Light, 1);
    m_ui->colorSchemeComboBox->setCurrentIndex(m_colorSchemeToIndex[m_ui->graphicsView->paletteMode()]);
    connect(m_ui->colorSchemeComboBox, &QComboBox::currentTextChanged,
            this, [this](const QString &)
    {
        m_ui->graphicsView->setPaletteMode(m_ui->colorSchemeComboBox->currentData().value<Palette::Mode>());
    });

    m_ui->rulerCheckBox->setChecked(m_ui->graphicsView->rulerEnabled());
    connect(m_ui->rulerCheckBox, &QCheckBox::toggled,
            this, [this](bool checked)
    {
        m_ui->graphicsView->setRulerEnabled(checked);
    });

    m_ui->gridCheckBox->setChecked(m_ui->graphicsView->gridEnabled());
    m_ui->gridGroupBox->setEnabled(m_ui->graphicsView->gridEnabled());
    connect(m_ui->gridCheckBox, &QCheckBox::toggled,
            this, [this](bool checked)
    {
        m_ui->gridGroupBox->setEnabled(checked);
        m_ui->graphicsView->setGridEnabled(checked);
    });

    updateGuiScrollBars();
    connect(m_ui->scrollBarsCheckBox, &QCheckBox::toggled,
            this, [this](bool)
    {
        updateViewScrollBars();
        updateGuiScrollBars();
    });
    connect(m_ui->scrollBarsAsNeededCheckBox, &QCheckBox::toggled,
            this, [this](bool)
    {
        updateViewScrollBars();
        updateGuiScrollBars();
    });

    updateGuiMouseCursor();
    connect(m_ui->cusorCrosshairCheckBox, &QCheckBox::toggled,
            this, [this](bool)
    {
        updateViewMouseCursor();
        updateGuiMouseCursor();
    });
    connect(m_ui->largeCursorCrosshairCheckBox, &QCheckBox::toggled,
            this, [this](bool)
    {
        updateViewMouseCursor();
        updateGuiMouseCursor();
    });

    updateGuiOriginMark();
    connect(m_ui->originCheckBox, &QCheckBox::toggled,
            this, [this](bool)
    {
        updateViewOriginMark();
        updateGuiOriginMark();
    });
    connect(m_ui->largeOriginAxisCheckBox, &QCheckBox::toggled,
            this, [this](bool)
    {
        updateViewOriginMark();
        updateGuiOriginMark();
    });

    m_ui->minimalGridSizeSpinBox->setValue(m_ui->graphicsView->minimalGridSize());
    connect(m_ui->minimalGridSizeSpinBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, [this](int value)
    {
        m_ui->graphicsView->setMinimalGridSize(value);
    });

    m_ui->coarseMultiplierSpinBox->setValue(m_ui->graphicsView->gridCoarseMultiplier());
    connect(m_ui->coarseMultiplierSpinBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, [this](int value)
    {
        m_ui->graphicsView->setGridCoarseMultiplier(value);
    });

    m_ui->solidCoarseGridLinesCheckBox->setChecked(m_ui->graphicsView->gridCoarseLineStyle() ==
                                                   Qt::SolidLine);
    connect(m_ui->solidCoarseGridLinesCheckBox, &QCheckBox::toggled,
            this, [this](bool checked)
    {
        if (checked)
        {
            m_ui->graphicsView->setGridCoarseLineStyle(Qt::SolidLine);
        }
        else
        {
            m_ui->graphicsView->setGridCoarseLineStyle(Qt::DotLine);
        }
    });

    m_ui->solidFineGridLinesCheckBox->setChecked(m_ui->graphicsView->gridFineLineStyle() ==
                                                 Qt::SolidLine);
    connect(m_ui->solidFineGridLinesCheckBox, &QCheckBox::toggled,
            this, [this](bool checked)
    {
        if (checked)
        {
            m_ui->graphicsView->setGridFineLineStyle(Qt::SolidLine);
        }
        else
        {
            m_ui->graphicsView->setGridFineLineStyle(Qt::DotLine);
        }
    });

    connect(m_ui->antiAliasingCheckBox, &QCheckBox::toggled,
            this, [this](bool checked)
    {
        m_ui->graphicsView->setRenderHint(QPainter::Antialiasing, checked);
        m_ui->graphicsView->setRenderHint(QPainter::TextAntialiasing, checked);
        m_ui->graphicsView->setRenderHint(QPainter::HighQualityAntialiasing, checked);
    });

    m_ui->openGlCheckBox->setChecked(m_ui->graphicsView->hardwareAccelerationEnabled());
    connect(m_ui->openGlCheckBox, &QCheckBox::toggled,
            this, [this](bool checked)
    {
        m_ui->graphicsView->setHardwareAccelerationEnabled(checked);
    });
}

SettingsWidget::~SettingsWidget()
{
    delete m_ui;
}

void SettingsWidget::loadSettings()
{
    Settings settings;
    settings.load(Core::settings());

    m_ui->colorSchemeComboBox->setCurrentIndex(m_colorSchemeToIndex.value(settings.colorScheme));
    m_ui->rulerCheckBox->setChecked(settings.rulerEnabled);
    m_ui->gridCheckBox->setChecked(settings.gridEnabled);
    m_ui->scrollBarsCheckBox->setChecked(settings.scrollBarsEnabled);
    m_ui->scrollBarsAsNeededCheckBox->setChecked(settings.scrollBarsAsNeededEnabled);
    m_ui->cusorCrosshairCheckBox->setChecked(settings.cursorCrosshairEnabled);
    m_ui->largeCursorCrosshairCheckBox->setChecked(settings.largeCursorCrosshairEnabled);
    m_ui->originCheckBox->setChecked(settings.originCrosshairEnabled);
    m_ui->largeOriginAxisCheckBox->setChecked(settings.largeOriginCrosshairEnabled);
    m_ui->minimalGridSizeSpinBox->setValue(int(settings.minimalGridSize));
    m_ui->coarseMultiplierSpinBox->setValue(int(settings.coarseGridMultiplier));
    m_ui->solidCoarseGridLinesCheckBox->setChecked(settings.solidCoarseGridLinesEnabled);
    m_ui->solidFineGridLinesCheckBox->setChecked(settings.solidFineGridLinesEnabled);
    m_ui->antiAliasingCheckBox->setChecked(settings.antiAliasingEnabled);
    m_ui->openGlCheckBox->setChecked(settings.hardwareAccelerationEnabled);
}

void SettingsWidget::saveSettings()
{
    Settings settings;

    settings.colorScheme = m_ui->colorSchemeComboBox->currentData().value<Palette::Mode>();
    settings.rulerEnabled = m_ui->rulerCheckBox->isChecked();
    settings.gridEnabled = m_ui->gridCheckBox->isChecked();
    settings.scrollBarsEnabled = m_ui->scrollBarsCheckBox->isChecked();
    settings.scrollBarsAsNeededEnabled = m_ui->scrollBarsAsNeededCheckBox->isChecked();
    settings.cursorCrosshairEnabled = m_ui->cusorCrosshairCheckBox->isChecked();
    settings.largeCursorCrosshairEnabled = m_ui->largeCursorCrosshairCheckBox->isChecked();
    settings.originCrosshairEnabled = m_ui->originCheckBox->isChecked();
    settings.largeOriginCrosshairEnabled = m_ui->largeOriginAxisCheckBox->isChecked();
    settings.minimalGridSize = uint(m_ui->minimalGridSizeSpinBox->value());
    settings.coarseGridMultiplier = uint(m_ui->coarseMultiplierSpinBox->value());
    settings.solidCoarseGridLinesEnabled = m_ui->solidCoarseGridLinesCheckBox->isChecked();
    settings.solidFineGridLinesEnabled = m_ui->solidFineGridLinesCheckBox->isChecked();
    settings.antiAliasingEnabled = m_ui->antiAliasingCheckBox->isChecked();
    settings.hardwareAccelerationEnabled = m_ui->openGlCheckBox->isChecked();

    settings.save(Core::settings());
}

void SettingsWidget::updateViewScrollBars()
{
    if (!m_ui->scrollBarsCheckBox->isChecked())
    {
        m_ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        m_ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }
    else if (m_ui->scrollBarsAsNeededCheckBox->isChecked())
    {
        m_ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        m_ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    }
    else
    {
        m_ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        m_ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    }
}

void SettingsWidget::updateGuiScrollBars()
{
    switch (m_ui->graphicsView->horizontalScrollBarPolicy())
    {
        case Qt::ScrollBarAlwaysOff:
            m_ui->scrollBarsCheckBox->setChecked(false);
            m_ui->scrollBarsAsNeededCheckBox->setChecked(false);
            break;
        case Qt::ScrollBarAsNeeded:
            m_ui->scrollBarsCheckBox->setChecked(true);
            m_ui->scrollBarsAsNeededCheckBox->setChecked(true);
            break;
        case Qt::ScrollBarAlwaysOn:
            m_ui->scrollBarsCheckBox->setChecked(true);
            m_ui->scrollBarsAsNeededCheckBox->setChecked(false);
            break;
    }
    m_ui->scrollBarsAsNeededCheckBox->setEnabled(m_ui->scrollBarsCheckBox->isChecked());
}

void SettingsWidget::updateViewMouseCursor()
{

    if (!m_ui->cusorCrosshairCheckBox->isChecked())
    {
        m_ui->graphicsView->setMouseCursor(View::NoMouseCursor);
    }
    else if (m_ui->largeCursorCrosshairCheckBox->isChecked())
    {
        m_ui->graphicsView->setMouseCursor(View::LargeMouseCursor);
    }
    else
    {
        m_ui->graphicsView->setMouseCursor(View::SmallMouseCursor);
    }
}

void SettingsWidget::updateGuiMouseCursor()
{
    switch (m_ui->graphicsView->mouseCursor())
    {
        case View::NoMouseCursor:
            m_ui->cusorCrosshairCheckBox->setChecked(false);
            m_ui->largeCursorCrosshairCheckBox->setChecked(false);
            break;
        case View::SmallMouseCursor:
            m_ui->cusorCrosshairCheckBox->setChecked(true);
            m_ui->largeCursorCrosshairCheckBox->setChecked(false);
            break;
        case View::LargeMouseCursor:
            m_ui->cusorCrosshairCheckBox->setChecked(true);
            m_ui->largeCursorCrosshairCheckBox->setChecked(true);
    }
    m_ui->largeCursorCrosshairCheckBox->setEnabled(m_ui->cusorCrosshairCheckBox->isChecked());
}

void SettingsWidget::updateViewOriginMark()
{
    if (!m_ui->originCheckBox->isChecked())
    {
        m_ui->graphicsView->setOriginMark(View::NoOriginMark);
    }
    else if (m_ui->largeOriginAxisCheckBox->isChecked())
    {
        m_ui->graphicsView->setOriginMark(View::LargeOriginMark);
    }
    else
    {
        m_ui->graphicsView->setOriginMark(View::SmallOriginMark);
    }
}

void SettingsWidget::updateGuiOriginMark()
{
    switch (m_ui->graphicsView->originMark())
    {
        case View::NoOriginMark:
            m_ui->originCheckBox->setChecked(false);
            m_ui->largeOriginAxisCheckBox->setChecked(false);
            break;
        case View::SmallOriginMark:
            m_ui->originCheckBox->setChecked(true);
            m_ui->largeOriginAxisCheckBox->setChecked(false);
            break;
        case View::LargeOriginMark:
            m_ui->originCheckBox->setChecked(true);
            m_ui->largeOriginAxisCheckBox->setChecked(true);
    }
    m_ui->largeOriginAxisCheckBox->setEnabled(m_ui->originCheckBox->isChecked());
}
