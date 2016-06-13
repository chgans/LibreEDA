#include "paletteloader.h"
#include "palette.h"

#include <QDir>
#include <QSet>

namespace SymbolEditor
{

    PaletteLoader::PaletteLoader(QObject *parent):
        QObject(parent),
        m_path("/home/krys/Projects/LibreEDA/src/plugins/symboleditor")
    {

    }

    QString PaletteLoader::path() const
    {
        return m_path;
    }

    void PaletteLoader::setPath(const QString &path)
    {
        QString newPath = path;
        newPath = newPath.replace(QChar(';'), QChar(':'));
        if (newPath == m_path)
        {
            return;
        }
        m_path = newPath;
        emit pathChanged(m_path);
    }

    void PaletteLoader::discover()
    {
        m_discoveredMap.clear();

        auto pathList = m_path.split(QChar(':'));
        for (auto path: pathList)
        {
            QDir dir(path);
            dir.setFilter(QDir::Files | QDir::Readable);
            dir.setNameFilters(QStringList() << "*.lesympalette");
            for (auto entry: dir.entryList())
            {
                Palette palette;
                if (!palette.load(dir.absoluteFilePath(entry)))
                {
                    continue;
                }
                m_discoveredMap.insert(palette.name(), palette);
            }
        }
    }

    void PaletteLoader::load(const QSet<QString> &names)
    {
        for (auto name: names)
        {
            auto palette = m_discoveredMap.value(name);
            m_loadedMap.insert(name, palette);
            emit paletteLoaded(name);
        }
    }

    void PaletteLoader::reload(const QSet<QString> &names)
    {
        for (auto name: names)
        {
            m_loadedMap.remove(name);
            auto palette = m_discoveredMap.value(name);
            m_loadedMap.insert(name, palette);
            emit paletteReloaded(name);
        }
    }

    void PaletteLoader::unload(const QSet<QString> &names)
    {
        for (auto name: names)
        {
            m_loadedMap.remove(name);
            emit paletteUnloaded(name);
        }
    }

    void PaletteLoader::loadPalettes()
    {
        discover();

        auto loaded = QSet<QString>::fromList(m_loadedMap.keys());
        auto discovered = QSet<QString>::fromList(m_discoveredMap.keys());

        auto toReload = loaded.intersect(discovered);
        auto toUnload = loaded - toReload;
        auto toLoad = discovered - toReload;

        unload(toUnload);
        reload(toReload);
        load(toLoad);
    }

    QStringList PaletteLoader::paletteNames() const
    {
        return m_loadedMap.keys();
    }

    Palette PaletteLoader::palette(const QString &name)
    {
        if (!m_loadedMap.contains(name))
        {
            return Palette();
        }

        return m_loadedMap.value(name);
    }

}
