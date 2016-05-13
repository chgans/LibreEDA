#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QObject>
#include <QList>
#include <QPointer>
#include <QHash>
#include <QSet>
#include <QReadWriteLock>

#include "core/core_global.h"

class QSettings;

class PluginSpec;
class PluginCollection;

class CORE_EXPORT PluginManager : public QObject
{
    Q_OBJECT
public:
    static PluginManager *instance();
    ~PluginManager();

    // Object pool operations
    static void addObject(QObject *obj);
    static void removeObject(QObject *obj);
    static QList<QObject *> allObjects();
    template <typename T> static QList<T *> getObjects()
    {
        QReadLocker lock(&m_lock);
        QList<T *> results;
        QList<QObject *> all = allObjects();
        for (QObject *obj : all)
        {
            T *result = qobject_cast<T *>(obj);
            if (result)
            {
                results += result;
            }
        }
        return results;
    }
    template <typename T, typename Predicate>
    static QList<T *> getObjects(Predicate predicate)
    {
        QReadLocker lock(&m_lock);
        QList<T *> results;
        QList<QObject *> all = allObjects();
        for (QObject *obj : all)
        {
            T *result = qobject_cast<T *>(obj);
            if (result && predicate(result))
            {
                results += result;
            }
        }
        return results;
    }
    template <typename T> static T *getObject()
    {
        QReadLocker lock(&m_lock);
        QList<QObject *> all = allObjects();
        for (QObject *obj : all)
        {
            if (T *result = qobject_cast<T *>(obj))
            {
                return result;
            }
        }
        return 0;
    }
    template <typename T, typename Predicate> static T *getObject(Predicate predicate)
    {
        QReadLocker lock(&m_lock);
        QList<QObject *> all = allObjects();
        for (QObject *obj : all)
        {
            if (T *result = qobject_cast<T *>(obj))
                if (predicate(result))
                {
                    return result;
                }
        }
        return 0;
    }

    static QObject *getObjectByName(const QString &name);
    static QObject *getObjectByClassName(const QString &className);

    // Plugin operations
    // TODO: make a friendly interface for the splash screen
    static int discoverPlugins();
    static void loadPlugins();
    static QStringList pluginPaths();
    static void setPluginPaths(const QStringList &paths);
    static QString pluginIID();
    static void setPluginIID(const QString &iid);
    static QList<PluginSpec *> plugins();
    static QHash<QString, PluginCollection *> pluginCollections();
    static bool hasError();
    static QSet<PluginSpec *> pluginsRequiringPlugin(PluginSpec *spec);
    static QSet<PluginSpec *> pluginsRequiredByPlugin(PluginSpec *spec);

    // Settings
    static void setSettings(QSettings *settings);
    static QSettings *settings();
    static void setGlobalSettings(QSettings *settings);
    static QSettings *globalSettings();
    static void writeSettings();

signals:
    void objectAdded(QObject *obj);
    void aboutToRemoveObject(QObject *obj);

    void pluginsChanged();
    void initializationDone();

public slots:
    void shutdown();

private:
    PluginManager();
    static QList<PluginSpec *> loadQueue();
    static bool loadQueue(PluginSpec *spec, QList<PluginSpec *> &queue,
                          QList<PluginSpec *> &circularityCheckQueue);
    static void resolveDependencies();
    static void readSettings();

    static PluginManager *m_instance;
    static QReadWriteLock m_lock;
    static QList<QObject *> m_objectPool;
    static QString m_pluginIID;
    static QStringList m_pluginPaths;
    static QList<PluginSpec *> m_pluginSpecs;
    static QHash<QString, PluginCollection *> m_pluginCollections;
    static PluginCollection *m_defaultPluginCollection;
    static QStringList m_defaultDisabledPlugins;
    static QStringList m_defaultEnabledPlugins;
    static QStringList m_disabledPlugins;
    static QStringList m_forceEnabledPlugins;
    static QSettings *m_settings;
    static QSettings *m_globalSettings;
};


#endif // PLUGINMANAGER_H
