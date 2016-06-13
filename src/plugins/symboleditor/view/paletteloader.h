#pragma once

#include "palette.h"

#include <QObject>
#include <QString>
#include <QMap>

namespace SymbolEditor
{

    class PaletteLoader : public QObject
    {
        Q_OBJECT

        Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged)

    public:
        explicit PaletteLoader(QObject *parent = 0);

        QString path() const;
        void setPath(const QString &path);

        void loadPalettes();

        QStringList paletteNames() const;

        Palette palette(const QString &name);

    signals:
        void pathChanged(const QString &path);
        void paletteLoaded(const QString &name);
        void paletteReloaded(const QString &name);
        void paletteUnloaded(const QString &name);

    public slots:

    protected:
        void discover();
        void load(const QSet<QString> &names);
        void reload(const QSet<QString> &names);
        void unload(const QSet<QString> &names);

    private:
        QString m_path;
        QMap<QString, Palette> m_loadedMap;
        QMap<QString, Palette> m_discoveredMap;
    };

}
