#include "extension.h"
#include "pluginmanager.h"
#include "pluginspec.h"
#include "plugincollection.h"

#include <QFileInfo>
#include <QFileInfoList>
#include <QDir>

Q_LOGGING_CATEGORY(ExtensionLoggingCategory, "leda.extension")

PluginManager *PluginManager::m_instance = nullptr;
QList<QObject *> PluginManager::m_objectPool;
QString PluginManager::m_pluginIID;
QStringList PluginManager::m_pluginPaths;
QList<PluginSpec *> PluginManager::m_pluginSpecs;
QHash<QString, PluginCollection *> PluginManager::m_pluginCollections;
PluginCollection *PluginManager::m_defaultPluginCollection;

PluginManager *PluginManager::instance()
{
    if (m_instance == nullptr)
        m_instance = new PluginManager();
    return m_instance;
}

PluginManager::PluginManager(): QObject()
{

}

PluginManager::~PluginManager()
{
    qDeleteAll(m_pluginSpecs);
    qDeleteAll(m_pluginCollections);
}

void PluginManager::addObject(QObject *obj)
{
    if (obj == nullptr) {
        ExtensionWarning() << "Trying to add a null object";
        return;
    }
    if (m_objectPool.contains(obj)) {
        ExtensionWarning() << "Trying to add an object multiple times:" << obj->objectName();
        return;
    }
    m_objectPool.append(obj);
}

void PluginManager::removeObject(QObject *obj)
{
    if (obj == nullptr) {
        ExtensionWarning() << "Trying to remove a null object";
        return;
    }
    if (m_objectPool.contains(obj)) {
        ExtensionWarning() << "Trying to remove an unknown object:" << obj->objectName();
        return;
    }
    m_objectPool.removeOne(obj);
}

QList<QObject *> PluginManager::allObjects()
{
    return m_objectPool;
}

QObject *PluginManager::getObjectByName(const QString &name)
{
    QList<QObject *> all = allObjects();
    foreach (QObject *obj, all) {
        if (obj->objectName() == name)
            return obj;
    }
    return nullptr;
}

// TBD: we need the invoke<> stuff as well, usefull to invoke
// slots on an object w/o #including it's class header
QObject *PluginManager::getObjectByClassName(const QString &className)
{
    const QByteArray ba = className.toUtf8();
    QList<QObject *> all = allObjects();
    foreach (QObject *obj, all) {
        if (obj->inherits(ba.constData()))
            return obj;
    }
    return 0;
}

int PluginManager::discoverPlugins()
{
    qDeleteAll(m_pluginCollections);
    qDeleteAll(m_pluginSpecs);
    m_pluginSpecs.clear();
    m_pluginCollections.clear();

    QStringList pluginFiles;
    QStringList searchPaths = m_pluginPaths;
    while (!searchPaths.isEmpty()) {
        const QDir dir(searchPaths.takeFirst());
        const QFileInfoList files = dir.entryInfoList(QDir::Files | QDir::NoSymLinks);
        foreach (const QFileInfo &file, files) {
            const QString filePath = file.absoluteFilePath();
            if (QLibrary::isLibrary(filePath))
                pluginFiles.append(filePath);
        }
        const QFileInfoList dirs = dir.entryInfoList(QDir::Dirs|QDir::NoDotAndDotDot);
        foreach (const QFileInfo &subdir, dirs)
            searchPaths << subdir.absoluteFilePath();
    }
    m_defaultPluginCollection = new PluginCollection(QString());
    m_pluginCollections.insert(QString(), m_defaultPluginCollection);

    foreach (const QString &pluginFile, pluginFiles) {
        PluginSpec *spec = new PluginSpec;
        if (!spec->read(pluginFile)) { // not a Libre EDA plugin
            ExtensionWarning() << QFileInfo(pluginFile).baseName() <<  "read failed with" << spec->errorString();
            delete spec;
            continue;
        }

        PluginCollection *collection = 0;
        // find correct plugin collection or create a new one
        if (m_pluginCollections.contains(spec->category())) {
            collection = m_pluginCollections.value(spec->category());
        } else {
            collection = new PluginCollection(spec->category());
            m_pluginCollections.insert(spec->category(), collection);
        }

        collection->addPlugin(spec);
        m_pluginSpecs.append(spec);
    }
    resolveDependencies();
    // ensure deterministic plugin load order by sorting
    qSort(m_pluginSpecs.begin(), m_pluginSpecs.end());
    emit instance()->pluginsChanged();
    return m_pluginSpecs.count();
}

QList<PluginSpec *> PluginManager::loadQueue()
{

}

void PluginManager::resolveDependencies()
{
    foreach (PluginSpec *spec, m_pluginSpecs) {
        spec->resolveDependencies(m_pluginSpecs);
    }
}

void PluginManager::loadPlugins()
{

}

void PluginManager::shutdown()
{

}

QStringList PluginManager::pluginPaths()
{
    return m_pluginPaths;
}

void PluginManager::setPluginPaths(const QStringList &paths)
{
    ExtensionDebug() << "Plugin search paths:" << paths;
    ExtensionDebug() << "Required IID:" << m_pluginIID;
    m_pluginPaths = paths;

}

QString PluginManager::pluginIID()
{
    return m_pluginIID;
}

void PluginManager::setPluginIID(const QString &iid)
{
    m_pluginIID = iid;
}

QList<PluginSpec *> PluginManager::plugins()
{
    return m_pluginSpecs;
}

QHash<QString, PluginCollection *> PluginManager::pluginCollections()
{
    return m_pluginCollections;
}

bool PluginManager::hasError()
{
    foreach (PluginSpec *spec, m_pluginSpecs) {
        if (spec->hasError())
            return true;
    }
    return false;
}

QSet<PluginSpec *> PluginManager::pluginsRequiringPlugin(PluginSpec *spec)
{

}

QSet<PluginSpec *> PluginManager::pluginsRequiredByPlugin(PluginSpec *spec)
{

}

#if 0
void PluginManager::setSettings(QSettings *settings)
{

}

QSettings *PluginManager::settings()
{

}

void PluginManager::setGlobalSettings(QSettings *settings)
{

}

QSettings *PluginManager::globalSettings()
{

}

void PluginManager::writeSettings()
{

}
#endif

