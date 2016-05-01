#ifndef DESIGNLAYERMANAGER_H
#define DESIGNLAYERMANAGER_H

#include <QObject>

#include "designlayer.h"
#include "designlayerset.h"

class QSettings;

/*
 * TODO:
 *  - error handling when loading/saving palettes
 *  - Make PaletteManager and LayerSetManager have a similar API
 *  - paletteand layer sets are qobject, managers take ownership
 */

class DesignLayerManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString systemPath READ systemPath WRITE setSystemPath NOTIFY systemPathChanged)
    Q_PROPERTY(QString userPath READ userPath WRITE setUserPath NOTIFY userPathChanged)

public:    
    ~DesignLayerManager();

    static DesignLayerManager *instance();

    QString systemPath() const;
    void setSystemPath(const QString &path);
    QString userPath() const;
    void setUserPath(const QString &path);
    void loadLayerSets();

    int count() const;
    QList<DesignLayerSet *> layerSets() const;
    void add(DesignLayerSet *set);
    void add(QList<DesignLayerSet *> sets);
    void remove(DesignLayerSet *set);
    void remove(QList<DesignLayerSet *> sets);

signals:
    void systemPathChanged(const QString &path);
    void userPathChanged(const QString &path);
    void layerSetAdded(DesignLayerSet *set);
    void layerSetRemoved(DesignLayerSet *set);

private:
    explicit DesignLayerManager(QObject *parent = nullptr);
    static DesignLayerManager *m_instance;
    QList<DesignLayerSet *> m_sets;
    QString m_systemPath;
    QString m_userPath;
    QList<DesignLayerSet *> loadLayerSets(const QString &path);
};


#endif // DESIGNLAYERMANAGER_H
