#include "pcbpalettemanager.h"
#include "pcbpalette.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QStringList>
#include <QSettings>
#include <QDebug>

PcbPaletteManager *PcbPaletteManager::m_instance = nullptr;

PcbPaletteManager::PcbPaletteManager(QObject *parent) :
    QObject(parent)
{
}

PcbPaletteManager *PcbPaletteManager::instance()
{
    if (m_instance == nullptr)
        m_instance = new PcbPaletteManager;
    return m_instance;
}

QList<PcbPalette *> PcbPaletteManager::palettes() const
{
    return m_palettes;
}

void PcbPaletteManager::add(PcbPalette *palette)
{
    add(QList<PcbPalette *>() << palette);
}

void PcbPaletteManager::add(QList<PcbPalette *> palettes)
{
    for (PcbPalette *palette: palettes) {
        if (!m_palettes.contains(palette)) {
            m_palettes.append(palette);
            emit paletteAdded(palette);
        }
    }
}

void PcbPaletteManager::remove(PcbPalette *palette)
{
    remove(QList<PcbPalette *>() << palette);
}

void PcbPaletteManager::remove(QList<PcbPalette *> palettes)
{
    for (PcbPalette *palette: palettes) {
        if (m_palettes.contains(palette)) {
            m_palettes.removeOne(palette);
            emit paletteRemoved(palette);
        }
    }
}

QString PcbPaletteManager::systemPath() const
{
    return m_systemPath;
}

void PcbPaletteManager::setSystemPath(const QString &path)
{
    if (path == m_systemPath)
        return;
    m_systemPath = path;
    emit systemPathChanged(m_systemPath);
}

QString PcbPaletteManager::userPath() const
{
    return m_userPath;
}

void PcbPaletteManager::setUserPath(const QString &path)
{
    if (path == m_userPath)
        return;
    m_userPath = path;
    emit userPathChanged(m_userPath);
}

void PcbPaletteManager::loadPalettes()
{
    qDebug() << "Palette manager: Loading palettes";
    m_palettes.clear();
    for (PcbPalette *palette: m_palettes)
       emit paletteRemoved(palette);
    qDeleteAll(m_palettes);
    m_palettes << loadPalettes(m_systemPath)
               << loadPalettes(m_userPath);
    for (PcbPalette *palette: m_palettes)
       emit paletteAdded(palette);
    qDebug() << "Palette manager: Loaded " << count() << "palettes";
}

int PcbPaletteManager::count() const
{
    return m_palettes.count();
}

QList<PcbPalette *> PcbPaletteManager::loadPalettes(const QString &path)
{
    QDir dir(path);
    if (path.isEmpty() || !dir.exists() || !dir.isReadable()) {
        qWarning() << "Palette manager: Skipping" << path << "=>" << dir.canonicalPath();
        return QList<PcbPalette *>();
    }
    qDebug() << "Palette manager: Scanning" << path << "=>" << dir.canonicalPath();
    QStringList filters;
    filters << "*.LedaPcbPalette";
    QList<PcbPalette *> palettes;
    for (QFileInfo fileInfo: dir.entryInfoList(filters)) {
        PcbPalette *palette = new PcbPalette();
        palettes.append(palette);
        QString id = fileInfo.baseName();
        QFile file(fileInfo.filePath());
        if (!file.open(QIODevice::ReadOnly))
            continue;
        file.close();
        QSettings settings(fileInfo.filePath(), QSettings::IniFormat);
        palette->loadFromSettings(settings);
        qDebug() << "Palette manager: Adding" << id;
    }
    return palettes;
}
