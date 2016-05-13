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

static const QColor BASE00("#657b83");
static const QColor BASE01("#586e75");
static const QColor BASE02("#073642");
static const QColor BASE03("#002b36");
static const QColor BASE0("#839496");
static const QColor BASE1("#93a1a1");
static const QColor BASE2("#eee8d5");
static const QColor BASE3("#fdf6e3");
static const QColor YELLOW("#b58900");
static const QColor ORANGE("#cb4b16");
static const QColor RED("#dc322f");
static const QColor MAGENTA("#d33682");
static const QColor VIOLET("#6c71c4");
static const QColor BLUE("#268bd2");
static const QColor CYAN("#2aa198");
static const QColor GREEN("#859900");


Palette::Palette(QObject *parent):
    QObject(parent),
    m_mode(Mode::Dark)
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

QColor Palette::background() const
{
    if (m_mode == Dark)
    {
        return BASE03;
    }
    else
    {
        return BASE3;
    }
}

QColor Palette::backgroundHighlight() const
{
    if (m_mode == Dark)
    {
        return BASE02;
    }
    else
    {
        return BASE2;
    }
}

QColor Palette::primaryContent() const
{
    if (m_mode == Dark)
    {
        return BASE0;
    }
    else
    {
        return BASE00;
    }
}

QColor Palette::secondaryContent() const
{
    if (m_mode == Dark)
    {
        return BASE01;
    }
    else
    {
        return BASE1;
    }
}

QColor Palette::emphasisedContent() const
{
    if (m_mode == Dark)
    {
        return BASE1;
    }
    else
    {
        return BASE01;
    }
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
