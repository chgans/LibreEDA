#pragma once

#include <QObject>
#include <QColor>

namespace SymbolEditor
{

    class Palette: public QObject
    {
        Q_OBJECT

        Q_ENUMS(Mode)

    public:
        explicit Palette(QObject *parent = nullptr);
        ~Palette();

        enum Mode
        {
            Dark = 0,
            Light
        };

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

        void setMode(Mode mode);
        Mode mode() const;

        QColor background() const;
        QColor backgroundHighlight() const;
        QColor primaryContent() const;
        QColor secondaryContent() const;
        QColor emphasisedContent() const;

        QColor yellow() const;
        QColor orange() const;
        QColor red() const;
        QColor magenta() const;
        QColor violet() const;
        QColor blue() const;
        QColor cyan() const;
        QColor green() const;

        QColor color(ColorId id) const;
    private:
        Mode m_mode;
    };

}

Q_DECLARE_METATYPE(SymbolEditor::Palette::Mode)

