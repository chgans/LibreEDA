#include "settings.h"

using namespace SymbolEditor;

// TODO: namespace Private
static const QString AppearanceGroupKey("SchEditor/Appearance");
static const QString ColorSchemeKey("ColorScheme");
static const QString RulerEnabledKey("RulerEnabled");
static const QString GridEnabledKey("GridEnabled");
static const QString ScrollBarsEnabledKey("ScrollBarsEnabled");
static const QString ScrollBarsAsNeededEnabledKey("ScrollBarsAsNeededEnabled");
static const QString CusorCrosshairEnabledKey("CusorCrosshairEnabled");
static const QString LargeCusorCrosshairEnabledKey("LargeCusorCrosshairEnabled");
static const QString OriginCrosshairEnabledKey("OriginEnabled");
static const QString LargeOriginCrosshairEnabledKey("LargeOriginEnabled");
static const QString MinimalGridSizeKey("MinimalGridSize");
static const QString CoarseGridMultiplierKey("CoarseGridMultiplier");
static const QString SolidCoarseGridLinesEnabledKey("SolidCoarseGridLinesEnabled");
static const QString SolidFineGridLinesEnabledKey("SolidFineGridLinesEnabled");
static const QString AntiAliasingEnabledKey("AntiAliasingEnabled");
static const QString HardwareAccelerationEnabledKey("HardwareAccelerationEnabled");


void Settings::load(QSettings *settings)
{
    settings->beginGroup(AppearanceGroupKey);
    QString colorSchemeString = settings->value(ColorSchemeKey, QString("Dark")).toString();
    if (colorSchemeString == "Light")
    {
        colorScheme = Palette::Light;
    }
    else
    {
        colorScheme = Palette::Dark;
    }
    rulerEnabled = settings->value(RulerEnabledKey, true).toBool();
    gridEnabled = settings->value(GridEnabledKey, true).toBool();
    scrollBarsEnabled = settings->value(ScrollBarsEnabledKey, true).toBool();
    scrollBarsAsNeededEnabled = settings->value(ScrollBarsAsNeededEnabledKey, true).toBool();
    cursorCrosshairEnabled = settings->value(CusorCrosshairEnabledKey, true).toBool();
    largeCursorCrosshairEnabled = settings->value(LargeCusorCrosshairEnabledKey, true).toBool();
    originCrosshairEnabled = settings->value(OriginCrosshairEnabledKey, true).toBool();
    largeOriginCrosshairEnabled = settings->value(LargeOriginCrosshairEnabledKey, true).toBool();
    minimalGridSize = settings->value(MinimalGridSizeKey, 10u).toUInt();
    coarseGridMultiplier = settings->value(CoarseGridMultiplierKey, 10u).toUInt();
    solidCoarseGridLinesEnabled = settings->value(SolidCoarseGridLinesEnabledKey, true).toBool();
    solidFineGridLinesEnabled = settings->value(SolidFineGridLinesEnabledKey, true).toBool();
    antiAliasingEnabled = settings->value(AntiAliasingEnabledKey, true).toBool();
    hardwareAccelerationEnabled = settings->value(HardwareAccelerationEnabledKey, true).toBool();
    settings->endGroup();
}

void Settings::save(QSettings *settings) const
{
    settings->beginGroup(AppearanceGroupKey);
    if (colorScheme == Palette::Dark)
    {
        settings->setValue(ColorSchemeKey, "Dark");
    }
    else
    {
        settings->setValue(ColorSchemeKey, "Light");
    }
    settings->setValue(RulerEnabledKey, rulerEnabled);
    settings->setValue(GridEnabledKey, gridEnabled);
    settings->setValue(ScrollBarsEnabledKey, scrollBarsEnabled);
    settings->setValue(ScrollBarsAsNeededEnabledKey, scrollBarsAsNeededEnabled);
    settings->setValue(CusorCrosshairEnabledKey, cursorCrosshairEnabled);
    settings->setValue(LargeCusorCrosshairEnabledKey, largeCursorCrosshairEnabled);
    settings->setValue(OriginCrosshairEnabledKey, originCrosshairEnabled);
    settings->setValue(LargeOriginCrosshairEnabledKey, largeOriginCrosshairEnabled);
    settings->setValue(MinimalGridSizeKey, minimalGridSize);
    settings->setValue(CoarseGridMultiplierKey, coarseGridMultiplier);
    settings->setValue(SolidCoarseGridLinesEnabledKey, solidCoarseGridLinesEnabled);
    settings->setValue(SolidFineGridLinesEnabledKey, solidFineGridLinesEnabled);
    settings->setValue(AntiAliasingEnabledKey, antiAliasingEnabled);
    settings->setValue(HardwareAccelerationEnabledKey, hardwareAccelerationEnabled);
    settings->endGroup();
    settings->sync();
}
