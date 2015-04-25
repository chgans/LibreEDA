#ifndef PCBPALETTEMANAGER_H
#define PCBPALETTEMANAGER_H

#include "pcbpalette.h"

#include <QObject>
#include <QMap>
#include <QString>

class PcbPaletteManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString systemPath READ systemPath WRITE setSystemPath NOTIFY systemPathChanged)
    Q_PROPERTY(QString userPath READ userPath WRITE setUserPath NOTIFY userPathChanged)

public:

    static PcbPaletteManager *instance();

    QString systemPath() const;
    void setSystemPath(const QString &path);
    QString userPath() const;
    void setUserPath(const QString &path);
    void loadPalettes();

    int count() const;
    QList<PcbPalette *> palettes() const;
    void add(PcbPalette *palette);
    void add(QList<PcbPalette *> palettes);
    void remove(PcbPalette *palette);
    void remove(QList<PcbPalette *> palettes);

signals:
    void systemPathChanged(const QString &path);
    void userPathChanged(const QString &path);
    void paletteAdded(PcbPalette *palette);
    void paletteRemoved(PcbPalette *palette);

public slots:

private:
    explicit PcbPaletteManager(QObject *parent = 0);
    static PcbPaletteManager *m_instance;
    QList<PcbPalette*> m_palettes;
    QString m_systemPath;
    QString m_userPath;
    QList<PcbPalette*> loadPalettes(const QString &path);
};

#endif // PCBPALETTEMANAGER_H
