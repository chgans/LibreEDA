#include "palette.h"

/* Solarized theme: http://ethanschoonover.com/solarized
$base03:    #002b36;
$base02:    #073642;
$base01:    #586e75;
$base00:    #657b83;
$base0:     #839496;
$base1:     #93a1a1;
$base2:     #eee8d5;
$base3:     #fdf6e3;
$yellow:    #b58900;
$orange:    #cb4b16;
$red:       #dc322f;
$magenta:   #d33682;
$violet:    #6c71c4;
$blue:      #268bd2;
$cyan:      #2aa198;
$green:     #859900;
*/

static const QColor BASED0("#657b83");
static const QColor BASED1("#586e75");
static const QColor BASED2("#073642");
static const QColor BASED3("#002b36");
static const QColor BASEL0("#839496");
static const QColor BASEL1("#93a1a1");
static const QColor BASEL2("#eee8d5");
static const QColor BASEL3("#fdf6e3");
static const QColor YELLOW("#b58900");
static const QColor ORANGE("#cb4b16");
static const QColor RED("#dc322f");
static const QColor MAGENTA("#d33682");
static const QColor VIOLET("#6c71c4");
static const QColor BLUE("#268bd2");
static const QColor CYAN("#2aa198");
static const QColor GREEN("#859900");


Palette::Palette(QObject *parent):
    QObject(parent)
{

}

Palette::~Palette()
{

}

void Palette::setMode(Palette::Mode mode)
{
    m_mode = mode;
}

Palette::Mode Palette::mode() const
{
    return m_mode;
}

QColor Palette::background1() const
{
    if (m_mode == Dark)
        return BASED3;
    else
        return BASEL3;
}

QColor Palette::background2() const
{
    if (m_mode == Dark)
        return BASED2;
    else
        return BASEL2;
}

QColor Palette::content1() const
{
    if (m_mode == Dark)
        return BASED1;
    else
        return BASEL1;
}

QColor Palette::content2() const
{
    if (m_mode == Dark)
        return BASED0;
    else
        return BASEL0;
}

QColor Palette::content3() const
{
    if (m_mode == Dark)
        return BASEL0;
    else
        return BASED0;
}

QColor Palette::content4() const
{
    if (m_mode == Dark)
        return BASEL1;
    else
        return BASED1;
}

QColor Palette::content5() const
{
    if (m_mode == Dark)
        return BASEL2;
    else
        return BASED2;
}

QColor Palette::content6() const
{
    if (m_mode == Dark)
        return BASEL3;
    else
        return BASED3;
}

QColor Palette::yellow() const
{
    return YELLOW;
}

QColor Palette::orange() const
{
    return ORANGE;
}

QColor Palette::red() const
{
    return RED;
}

QColor Palette::magenta() const
{
    return MAGENTA;
}

QColor Palette::violet() const
{
    return VIOLET;
}

QColor Palette::blue() const
{
    return BLUE;
}

QColor Palette::cyan() const
{
    return CYAN;
}

QColor Palette::green() const
{
    return GREEN;
}
