#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QObject>
#include <QList>
#include <QPointer>
#include <QHash>
#include <QSet>

class QSettings;

class PluginSpec;
class PluginCollection;

class PluginManager : public QObject
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
        QList<T *> results;
        QList<QObject *> all = allObjects();
        foreach (QObject *obj, all) {
            T *result = qobject_cast<T *>(obj);
            if (result)
                results += result;
        }
        return results;
    }
    template <typename T, typename Predicate>
    static QList<T *> getObjects(Predicate predicate)
    {
        QList<T *> results;
        QList<QObject *> all = allObjects();
        foreach (QObject *obj, all) {
            T *result = qobject_cast<T *>(obj);
            if (result && predicate(result))
                results += result;
        }
        return results;
    }
    template <typename T> static T *getObject()
    {
        QList<QObject *> all = allObjects();
        foreach (QObject *obj, all) {
            if (T *result = qobject_cast<T *>(obj))
                return result;
        }
        return 0;
    }
    template <typename T, typename Predicate> static T *getObject(Predicate predicate)
    {
        QList<QObject *> all = allObjects();
        foreach (QObject *obj, all) {
            if (T *result = qobject_cast<T *>(obj))
                if (predicate(result))
                    return result;
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
#if 0
    static void setSettings(QSettings *settings);
    static QSettings *settings();
    static void setGlobalSettings(QSettings *settings);
    static QSettings *globalSettings();
    static void writeSettings();
#endif

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
    static void resolveDependencies();

    static PluginManager *m_instance;
    static QList<QObject *> m_objectPool;
    static QString m_pluginIID;
    static QStringList m_pluginPaths;
    static QList<PluginSpec *> m_pluginSpecs;
    static QHash<QString, PluginCollection *> m_pluginCollections;
    static PluginCollection *m_defaultPluginCollection;
};


#endif // PLUGINMANAGER_H
