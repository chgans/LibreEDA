#pragma once

#include <QtGlobal>
#include <QSettings>
#include "view/palette.h"

namespace SymbolEditor
{

    class Settings
    {
    public:
        QString paletteName = "";
        bool rulerEnabled = true;
        bool gridEnabled = true;
        bool scrollBarsEnabled = false;
        bool scrollBarsAsNeededEnabled = false;
        bool cursorCrosshairEnabled = true;
        bool largeCursorCrosshairEnabled = true;
        bool originCrosshairEnabled = true;
        bool largeOriginCrosshairEnabled = true;
        uint minimalGridSize = 10;
        uint coarseGridMultiplier = 10;
        bool solidCoarseGridLinesEnabled = true;
        bool solidFineGridLinesEnabled = false;
        bool antiAliasingEnabled = false;
        bool hardwareAccelerationEnabled = false;

        void load(QSettings *settings);
        void save(QSettings *settings) const;
    };

}
