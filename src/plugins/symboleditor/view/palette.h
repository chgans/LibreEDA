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

    private:
        Mode m_mode;
    };

}

Q_DECLARE_METATYPE(SymbolEditor::Palette::Mode)

