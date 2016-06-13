#pragma once

#include <QColor>
#include <QMap>

namespace SymbolEditor
{

    class Palette
    {
    public:

        // Match xdl::symbol::Color enum
        enum ColorId
        {
            EmphasisedContent = 0,
            PrimaryContent = 1,
            SecondaryContent = 2,
            BackgroundHighlight = 3,
            Background = 4,
            Yellow = 5,
            Orange = 6,
            Red = 7,
            Magenta = 8,
            Violet = 9,
            Blue = 10,
            Cyan = 11,
            Green = 12
        };

        Palette();
        ~Palette();

        Palette(const Palette &other);
        Palette& operator = (const Palette&other);

        bool load(const QString &fileName, QString *error = nullptr);
        bool save(const QString &fileName, QString *error = nullptr);

        QString name() const;
        void setName(const QString &name);

        QColor background() const;
        void setBackground(const QColor &color);
        QColor backgroundHighlight() const;
        void setBackgroundHighlight(const QColor &color);

        QColor primaryContent() const;
        void setPrimaryContent(const QColor &color);
        QColor secondaryContent() const;
        void setSecondaryContent(const QColor &color);
        QColor emphasisedContent() const;
        void setEmphasisedContent(const QColor &color);

        QColor yellow() const;
        void setYellow(const QColor &color);
        QColor orange() const;
        void setOrange(const QColor &color);
        QColor red() const;
        void setRed(const QColor &color);
        QColor magenta() const;
        void setMagenta(const QColor &color);
        QColor violet() const;
        void setViolet(const QColor &color);
        QColor blue() const;
        void setBlue(const QColor &color);
        QColor cyan() const;
        void setCyan(const QColor &color);
        QColor green() const;
        void setGreen(const QColor &color);

        QColor color(ColorId id) const;
        void setColor(ColorId id, const QColor &color);

    private:
        QString m_name;
        QMap<ColorId, QColor> m_palette;
    };

}


