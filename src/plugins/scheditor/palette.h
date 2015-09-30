#ifndef PALETTE_H
#define PALETTE_H

#include <QObject>
#include <QColor>

class Palette: public QObject
{
    Q_ENUMS(Mode)

public:
    Palette(QObject *parent = nullptr);
    ~Palette();

    enum Mode {
        Dark = 0,
        Light
    };

    void setMode(Mode mode);
    Mode mode() const;

    QColor background1() const;
    QColor background2() const;
    QColor content1() const;
    QColor content2() const;
    QColor content3() const;
    QColor content4() const;
    QColor content5() const;
    QColor content6() const;

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

Q_DECLARE_METATYPE(Palette::Mode)

#endif // PALETTE_H

