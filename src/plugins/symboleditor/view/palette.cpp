#include "palette.h"

#include <QSettings>
#include <QFile>

namespace SymbolEditor
{

    Palette::Palette()
    {
        m_name = "No name";
        m_palette[EmphasisedContent] = QColor("dimgrey");
        m_palette[PrimaryContent] = QColor("grey");
        m_palette[SecondaryContent] = QColor("lightgrey");
        m_palette[BackgroundHighlight] = QColor("lightgrey");
        m_palette[Background] = QColor("white");
        m_palette[Yellow] = QColor("yellow");
        m_palette[Orange] = QColor("orange");
        m_palette[Red] = QColor("red");
        m_palette[Magenta] = QColor("magenta");
        m_palette[Violet] = QColor("violet");
        m_palette[Blue] = QColor("blue");
        m_palette[Cyan] = QColor("cyan");
        m_palette[Green] = QColor("green");
    }

    Palette::~Palette()
    {

    }

    Palette::Palette(const Palette &other):
        m_name(other.m_name),
        m_palette(other.m_palette)
    {

    }

    Palette &Palette::operator =(const Palette &other)
    {
        m_name = other.m_name;
        m_palette = other.m_palette;
        return *this;
    }

    // TODO: check for errors
    bool Palette::load(const QString &fileName, QString *error)
    {
        QSettings settings(fileName, QSettings::IniFormat);
        settings.beginGroup("leda.symbol.palette");
        setName(settings.value("PaletteName", name()).toString());
        setEmphasisedContent(QColor(settings.value("EmphasisedContent", emphasisedContent()).toString()));
        setPrimaryContent(QColor(settings.value("PrimaryContent", primaryContent()).toString()));
        setSecondaryContent(QColor(settings.value("SecondaryContent", secondaryContent()).toString()));
        setBackgroundHighlight(QColor(settings.value("BackgroundHighlight", backgroundHighlight()).toString()));
        setBackground(QColor(settings.value("Background", background()).toString()));
        setYellow(QColor(settings.value("Yellow", yellow()).toString()));
        setOrange(QColor(settings.value("Orange", orange()).toString()));
        setRed(QColor(settings.value("Red", red()).toString()));
        setMagenta(QColor(settings.value("Magenta", magenta()).toString()));
        setViolet(QColor(settings.value("Violet", violet()).toString()));
        setBlue(QColor(settings.value("Blue", blue()).toString()));
        setCyan(QColor(settings.value("Cyan", cyan()).toString()));
        setGreen(QColor(settings.value("Green", green()).toString()));
        settings.endGroup();
        settings.sync();

        if (error != nullptr)
        {
            *error = "";
        }

        return true;
    }

    bool Palette::save(const QString &fileName, QString *error)
    {
        if (!QFile::exists(fileName))
        {
            QFile file(fileName);
            file.open(QIODevice::WriteOnly);
            file.write(" ");
            file.close();
        }

        QSettings settings(fileName, QSettings::IniFormat);
        settings.beginGroup("leda.symbol.palette");
        settings.setValue("PaletteName", name());
        settings.setValue("EmphasisedContent", emphasisedContent().name());
        settings.setValue("PrimaryContent", primaryContent().name());
        settings.setValue("SecondaryContent", secondaryContent().name());
        settings.setValue("BackgroundHighlight", backgroundHighlight().name());
        settings.setValue("Background", background().name());
        settings.setValue("Yellow", yellow().name());
        settings.setValue("Orange", orange().name());
        settings.setValue("Red", red().name());
        settings.setValue("Magenta", magenta().name());
        settings.setValue("Violet", violet().name());
        settings.setValue("Blue", blue().name());
        settings.setValue("Cyan", cyan().name());
        settings.setValue("Green", green().name());
        settings.endGroup();
        settings.sync();

        if (error != nullptr)
        {
            *error = "";
        }

        return true;
    }

    QString Palette::name() const
    {
        return m_name;
    }

    void Palette::setName(const QString &name)
    {
        m_name = name;
    }

    QColor Palette::background() const
    {
        return m_palette.value(Background);
    }

    void Palette::setBackground(const QColor &color)
    {
        m_palette[Background] = color;
    }

    QColor Palette::backgroundHighlight() const
    {
        return m_palette.value(BackgroundHighlight);
    }

    void Palette::setBackgroundHighlight(const QColor &color)
    {
        m_palette[BackgroundHighlight] = color;
    }

    QColor Palette::primaryContent() const
    {
        return m_palette.value(PrimaryContent);
    }

    void Palette::setPrimaryContent(const QColor &color)
    {
        m_palette[PrimaryContent] = color;
    }

    QColor Palette::secondaryContent() const
    {
        return m_palette.value(SecondaryContent);
    }

    void Palette::setSecondaryContent(const QColor &color)
    {
        m_palette[SecondaryContent] = color;
    }

    QColor Palette::emphasisedContent() const
    {
        return m_palette.value(EmphasisedContent);
    }

    void Palette::setEmphasisedContent(const QColor &color)
    {
        m_palette[EmphasisedContent] = color;
    }

    QColor Palette::yellow() const
    {
        return m_palette.value(Yellow);
    }

    void Palette::setYellow(const QColor &color)
    {
        m_palette[Yellow] = color;
    }

    QColor Palette::orange() const
    {
        return m_palette.value(Orange);
    }

    void Palette::setOrange(const QColor &color)
    {
        m_palette[Orange] = color;
    }

    QColor Palette::red() const
    {
        return m_palette.value(Red);
    }

    void Palette::setRed(const QColor &color)
    {
        m_palette[Red] = color;
    }

    QColor Palette::magenta() const
    {
        return m_palette.value(Magenta);
    }

    void Palette::setMagenta(const QColor &color)
    {
        m_palette[Magenta] = color;
    }

    QColor Palette::violet() const
    {
        return m_palette.value(Violet);
    }

    void Palette::setViolet(const QColor &color)
    {
        m_palette[Violet] = color;
    }

    QColor Palette::blue() const
    {
        return m_palette.value(Blue);
    }

    void Palette::setBlue(const QColor &color)
    {
        m_palette[Blue] = color;
    }

    QColor Palette::cyan() const
    {
        return m_palette.value(Cyan);
    }

    void Palette::setCyan(const QColor &color)
    {
        m_palette[Cyan] = color;
    }

    QColor Palette::green() const
    {
        return m_palette.value(Green);
    }

    void Palette::setGreen(const QColor &color)
    {
        m_palette[Green] = color;
    }

    QColor Palette::color(Palette::ColorId id) const
    {
        return m_palette.value(id);
    }

    void Palette::setColor(Palette::ColorId id, const QColor &color)
    {
        m_palette[id] = color;
    }

}

#if 0

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
#endif
