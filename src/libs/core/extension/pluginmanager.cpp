#include "extension.h"
#include "pluginmanager.h"
#include "pluginspec.h"
#include "plugincollection.h"

#include <QFileInfo>
#include <QFileInfoList>
#include <QDir>
#include <QReadLocker>
#include <QSettings>

Q_LOGGING_CATEGORY(ExtensionLoggingCategory, "leda.extension")

PluginManager *PluginManager::m_instance = nullptr;
QReadWriteLock PluginManager::m_lock;
QList<QObject *> PluginManager::m_objectPool;
QString PluginManager::m_pluginIID;
QStringList PluginManager::m_pluginPaths;
QList<PluginSpec *> PluginManager::m_pluginSpecs;
QHash<QString, PluginCollection *> PluginManager::m_pluginCollections;
PluginCollection *PluginManager::m_defaultPluginCollection;
QStringList PluginManager::m_defaultDisabledPlugins;
QStringList PluginManager::m_defaultEnabledPlugins;
QStringList PluginManager::m_disabledPlugins;
QStringList PluginManager::m_forceEnabledPlugins;
QSettings *PluginManager::m_settings = nullptr;
QSettings *PluginManager::m_globalSettings = nullptr;

static const QString S_IGNORED_PLUGINS = QStringLiteral("Plugins/Ignored");
static const QString S_FORCEENABLED_PLUGINS = QStringLiteral("Plugins/ForceEnabled");
/*!
    \class PluginManager
    \inmodule LibreEDA
    \ingroup LeExtensionSystem
    \preliminary
    \mainclass

    \brief The PluginManager class implements the core plugin system that
    manages the plugins, their life cycle, and their registered objects.

    The plugin manager is used for the following tasks:
    \list
    \li Manage plugins and their state
    \li Manipulate a 'common object pool'
    \endlist

    \section1 Plugins
    Plugins consist of a JSON descriptor file, and of a library that contains a Qt plugin
    that must derive from the IPlugin class and has an IID of
    \c "org.libre-eda.leda.plugin".
    The plugin manager is used to set a list of file system directories to search for
    plugins, retrieve information about the state of these plugins, and to load them.

    Usually, the application sets up the PluginManager and initiates the
    loading.
    \code
        // 'plugins' and subdirs will be searched for plugins
        PluginManager::setPluginPaths(QStringList() << "plugins");
        PluginManager::discoverPlugins(); // scan for plugins
        PluginManager::loadPlugins(); // try to load all the plugins
    \endcode
    discoverPlugins() return the number of plugins found and valid,
    calling loadPlugins() will trigger pluginsChanged() signals as
    all the plugins get loaded, this can be use to show the loading state using a
    splashcreen for example.
    Additionally, it is possible to directly access the plugin specifications
    (the information in the descriptor file), the plugin instances (via PluginSpec),
    and their state.

    \section1 Object Pool
    Plugins (and everybody else) can add objects to a common 'pool' that is located in
    the plugin manager. Objects in the pool must derive from QObject, there are no other
    prerequisites. All objects of a specified type can be retrieved from the object pool
    via the getObjects() and getObject() functions.

    Whenever the state of the object pool changes a corresponding signal is emitted by the plugin manager.

    A common usecase for the object pool is that a plugin (or the application) provides
    an "extension point" for other plugins, which is a class / interface that can
    be implemented and added to the object pool. The plugin that provides the
    extension point looks for implementations of the class / interface in the object pool.
    \code
        // Plugin A provides a "MimeTypeHandler" extension point
        // in plugin B:
        MyMimeTypeHandler *handler = new MyMimeTypeHandler();
        PluginManager::instance()->addObject(handler);
        // In plugin A:
        QList<MimeTypeHandler *> mimeHandlers =
            PluginManager::getObjects<MimeTypeHandler>();
    \endcode


    The \c{ExtensionSystem::Invoker} class template provides "syntactic sugar"
    for using "soft" extension points that may or may not be provided by an
    object in the pool. This approach does neither require the "user" plugin being
    linked against the "provider" plugin nor a common shared
    header file. The exposed interface is implicitly given by the
    invokable functions of the "provider" object in the object pool.

    The \c{ExtensionSystem::invoke} function template encapsulates
    {ExtensionSystem::Invoker} construction for the common case where
    the success of the call is not checked.

    \code
        // In the "provide" plugin A:
        namespace PluginA {
        class SomeProvider : public QObject
        {
            Q_OBJECT

        public:
            Q_INVOKABLE QString doit(const QString &msg, int n) {
            {
                qDebug() << "I AM DOING IT " << msg;
                return QString::number(n);
            }
        };
        } // namespace PluginA


        // In the "user" plugin B:
        int someFuntionUsingPluginA()
        {
            using namespace ExtensionSystem;

            QObject *target = PluginManager::getObjectByClassName("PluginA::SomeProvider");

            if (target) {
                // Some random argument.
                QString msg = "REALLY.";

                // Plain function call, no return value.
                invoke<void>(target, "doit", msg, 2);

                // Plain function with no return value.
                qDebug() << "Result: " << invoke<QString>(target, "doit", msg, 21);

                // Record success of function call with return value.
                Invoker<QString> in1(target, "doit", msg, 21);
                qDebug() << "Success: (expected)" << in1.wasSuccessful();

                // Try to invoke a non-existing function.
                Invoker<QString> in2(target, "doitWrong", msg, 22);
                qDebug() << "Success (not expected):" << in2.wasSuccessful();

            } else {

                // We have to cope with plugin A's absence.
            }
        };
    \endcode

    \note The type of the parameters passed to the \c{invoke()} calls
    is deduced from the parameters themselves and must match the type of
    the arguments of the called functions \e{exactly}. No conversion or even
    integer promotions are applicable, so to invoke a function with a \c{long}
    parameter explicitly use \c{long(43)} or such.

    \note The object pool manipulating functions are thread-safe.
*/

/*!
    \fn void PluginManager::objectAdded(QObject *obj)
    Signals that \a obj has been added to the object pool.
*/

/*!
    \fn void PluginManager::aboutToRemoveObject(QObject *obj)
    Signals that \a obj will be removed from the object pool.
*/

/*!
    \fn void PluginManager::pluginsChanged()
    Signals that the list of available plugins has changed.

    \sa plugins()
*/

/*!
    \fn void PluginManager::initializationDone()
    Signals that the initialisation of all available plugins has finished.
*/

/*!
    \fn T *PluginManager::getObject()

    Retrieves the object of a given type from the object pool.

    This function uses \c qobject_cast to determine the type of an object.
    If there are more than one object of the given type in
    the object pool, this function will arbitrarily choose one of them.

    \sa addObject()
*/

/*!
    \fn T *PluginManager::getObject(Predicate predicate)

    Retrieves the object of a given type from the object pool that matches
    the \a predicate.

    This function uses \c qobject_cast to determine the type of an object.
    The predicate must be a function taking T * and returning a bool.
    If there is more than one object matching the type and predicate,
    this function will arbitrarily choose one of them.

    \sa addObject()
*/

/*!
    \fn QList<T *> PluginManager::getObjects()

    Retrieves all objects of a given type from the object pool.

    This function uses \c qobject_cast to determine the type of an object.

    \sa addObject()
*/

/*!
    \fn QList<T *> PluginManager::getObjects(Predicate predicate)

    Retrieves all objects of a given type from the object pool that
    match the \a predicate.

    This function uses \c qobject_cast to determine the type of an object.
    The predicate should be a unary function taking a T* parameter and
    returning a bool.

    \sa addObject()
*/


/*!
    Gets the unique plugin manager instance.
*/
PluginManager *PluginManager::instance()
{
    if (m_instance == nullptr)
    {
        m_instance = new PluginManager();
    }
    return m_instance;
}

/*!
    \internal
*/
PluginManager::PluginManager(): QObject()
{

}

/*!
    \internal
*/
PluginManager::~PluginManager()
{
    qDeleteAll(m_pluginSpecs);
    qDeleteAll(m_pluginCollections);
}

/*!
    Adds the object \a obj to the object pool, so it can be retrieved
    again from the pool by type.

    The plugin manager does not do any memory management - added objects
    must be removed from the pool and deleted manually by whoever is responsible for the object.

    Emits the objectAdded() signal.

    \sa removeObject()
    \sa getObject()
    \sa getObjects()
*/
void PluginManager::addObject(QObject *obj)
{
    {
        QWriteLocker lock(&m_lock);
        if (obj == nullptr)
        {
            ExtensionWarning() << "Trying to add a null object";
            return;
        }
        if (m_objectPool.contains(obj))
        {
            ExtensionWarning() << "Trying to add an object multiple times:" << obj->objectName();
            return;
        }
        m_objectPool.append(obj);
    }
    emit instance()->objectAdded(obj);
}

/*!
    Emits aboutToRemoveObject() and removes the object \a obj from the object pool.
    \sa addObject()
*/
void PluginManager::removeObject(QObject *obj)
{
    if (obj == nullptr)
    {
        ExtensionWarning() << "Trying to remove a null object";
        return;
    }
    if (m_objectPool.contains(obj))
    {
        ExtensionWarning() << "Trying to remove an unknown object:" << obj->objectName();
        return;
    }
    emit instance()->aboutToRemoveObject(obj);
    QWriteLocker lock(&m_lock);
    m_objectPool.removeAll(obj);
}

/*!
    Retrieves the list of all objects in the pool, unfiltered.

    Usually, clients do not need to call this function.

    \sa getObject()
    \sa getObjects()
*/
QList<QObject *> PluginManager::allObjects()
{
    return m_objectPool;
}

/*!
    Retrieves one object with \a name from the object pool.

    \sa addObject()
*/
QObject *PluginManager::getObjectByName(const QString &name)
{
    QReadLocker lock(&m_lock);
    QList<QObject *> all = allObjects();
    for (QObject *obj : all)
    {
        if (obj->objectName() == name)
        {
            return obj;
        }
    }
    return nullptr;
}

/*!
    Retrieves one object inheriting a class with \a className from the object
    pool.

    \sa addObject()
*/
// Todo: we need the invoke<> stuff as well
QObject *PluginManager::getObjectByClassName(const QString &className)
{
    const QByteArray ba = className.toUtf8();
    QReadLocker lock(&m_lock);
    QList<QObject *> all = allObjects();
    for (QObject *obj : all)
    {
        if (obj->inherits(ba.constData()))
        {
            return obj;
        }
    }
    return 0;
}

/*!
    Scan the plugin search paths for valid plugins. The plugin specs of the plugins
    can be used to retrieve error and state information about individual plugins.

    \sa setPluginPaths()
    \sa plugins()
*/
int PluginManager::discoverPlugins()
{
    qDeleteAll(m_pluginCollections);
    qDeleteAll(m_pluginSpecs);
    m_pluginSpecs.clear();
    m_pluginCollections.clear();

    QStringList pluginFiles;
    QStringList searchPaths = m_pluginPaths;
    while (!searchPaths.isEmpty())
    {
        const QDir dir(searchPaths.takeFirst());
        const QFileInfoList files = dir.entryInfoList(QDir::Files | QDir::NoSymLinks);
        for (const QFileInfo &file : files)
        {
            const QString filePath = file.absoluteFilePath();
            if (QLibrary::isLibrary(filePath))
            {
                pluginFiles.append(filePath);
            }
        }
        const QFileInfoList dirs = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
        for (const QFileInfo &subdir : dirs)
        {
            searchPaths << subdir.absoluteFilePath();
        }
    }
    m_defaultPluginCollection = new PluginCollection(QString());
    m_pluginCollections.insert(QString(), m_defaultPluginCollection);

    for (const QString &pluginFile : pluginFiles)
    {
        PluginSpec *spec = new PluginSpec;
        if (!spec->read(pluginFile))   // not a Libre EDA plugin
        {
            ExtensionWarning() << QFileInfo(pluginFile).baseName() <<  "read failed with" <<
                               spec->errorString();
            delete spec;
            continue;
        }

        PluginCollection *collection = 0;
        // find correct plugin collection or create a new one
        if (m_pluginCollections.contains(spec->category()))
        {
            collection = m_pluginCollections.value(spec->category());
        }
        else
        {
            collection = new PluginCollection(spec->category());
            m_pluginCollections.insert(spec->category(), collection);
        }

        // defaultDisabledPlugins and defaultEnabledPlugins from install settings
        // is used to override the defaults read from the plugin spec
        if (spec->isEnabledByDefault() && m_defaultDisabledPlugins.contains(spec->name()))
        {
            spec->setEnabledByDefault(false);
            spec->setEnabledBySettings(false);
        }
        else if (!spec->isEnabledByDefault() && m_defaultEnabledPlugins.contains(spec->name()))
        {
            spec->setEnabledByDefault(true);
            spec->setEnabledBySettings(true);
        }
        if (!spec->isEnabledByDefault() && m_forceEnabledPlugins.contains(spec->name()))
        {
            spec->setEnabledBySettings(true);
        }
        if (spec->isEnabledByDefault() && m_disabledPlugins.contains(spec->name()))
        {
            spec->setEnabledBySettings(false);
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

/*!
    \internal
 */
QList<PluginSpec *> PluginManager::loadQueue()
{
    QList<PluginSpec *> queue;
    for (PluginSpec *spec : m_pluginSpecs)
    {
        QList<PluginSpec *> circularityCheckQueue;
        loadQueue(spec, queue, circularityCheckQueue);
    }
    return queue;
}

/*!
    \internal
 */
bool PluginManager::loadQueue(PluginSpec *spec, QList<PluginSpec *> &queue,
                              QList<PluginSpec *> &circularityCheckQueue)
{

    if (queue.contains(spec))
    {
        return true;
    }
    // check for circular dependencies
    if (circularityCheckQueue.contains(spec))
    {
        QString error;
        error = PluginManager::tr("Circular dependency detected:");
        error += QLatin1Char('\n');
        int index = circularityCheckQueue.indexOf(spec);
        for (int i = index; i < circularityCheckQueue.size(); ++i)
        {
            error.append(PluginManager::tr("%1(%2) depends on")
                         .arg(circularityCheckQueue.at(i)->name()).arg(circularityCheckQueue.at(i)->version()));
            error += QLatin1Char('\n');
        }
        error.append(PluginManager::tr("%1(%2)").arg(spec->name()).arg(spec->version()));
        return spec->reportError(error);
    }
    circularityCheckQueue.append(spec);
    // check if we have the dependencies
    if (spec->state() == PluginSpec::Invalid || spec->state() == PluginSpec::Read)
    {
        queue.append(spec);
        return false;
    }

    // add dependencies
    QHashIterator<PluginDependency, PluginSpec *> it(spec->dependencySpecs());
    while (it.hasNext())
    {
        it.next();
        // Skip test dependencies since they are not real dependencies but just force-loaded
        // plugins when running tests
        if (it.key().type == PluginDependency::Test)
        {
            continue;
        }
        PluginSpec *depSpec = it.value();
        if (!loadQueue(depSpec, queue, circularityCheckQueue))
        {
            QString error =
                PluginManager::tr("Cannot load plugin because dependency failed to load: %1(%2)\nReason: %3")
                .arg(depSpec->name()).arg(depSpec->version()).arg(depSpec->errorString());
            return spec->reportError(error);
        }
    }
    // add self
    queue.append(spec);
    return true;
}

/*!
    \internal
 */
void PluginManager::resolveDependencies()
{
    for (PluginSpec *spec : m_pluginSpecs)
    {
        spec->resolveDependencies(m_pluginSpecs);
    }
}

/*!
    Tries to load all the plugins that were previously found when
    calling discoverPlugins(). The plugin specs of the plugins
    can be used to retrieve error and state information about individual plugins.

    \sa setPluginPaths()
    \sa plugins()
*/
void PluginManager::loadPlugins()
{
    loadQueue();
    resolveDependencies();
    for (PluginSpec *spec : m_pluginSpecs)
    {
        spec->load();
        spec->initialise();
    }
}

/*!
    Shuts down and deletes all plugins.
*/
void PluginManager::shutdown()
{

}

/*!
    The list of paths were the plugin manager searches for plugins.

    \sa setPluginPaths()
*/
QStringList PluginManager::pluginPaths()
{
    return m_pluginPaths;
}

/*!
    Sets the plugin search paths, i.e. the file system paths where the plugin manager
    looks for plugin descriptions. All given \a paths and their sub directory trees
    are searched for plugin description files.

    At the moment this must be called before discoverPlugins() is called.

    \sa pluginPaths()
    \sa discoverPlugins()
    \sa loadPlugins()
*/
void PluginManager::setPluginPaths(const QStringList &paths)
{
    ExtensionDebug() << "Plugin search paths:" << paths;
    ExtensionDebug() << "Required IID:" << m_pluginIID;
    m_pluginPaths = paths;

}

/*!
    The IID that valid plugins must have.

    \sa setPluginIID()
*/
QString PluginManager::pluginIID()
{
    return m_pluginIID;
}

/*!
    Sets the IID that valid plugins must have. Only plugins with this \a iid are loaded, others are
    silently ignored.

    At the moment this must be called before discoverPlugins() is called.
*/
void PluginManager::setPluginIID(const QString &iid)
{
    m_pluginIID = iid;
}

/*!
    Returns the list of all plugin specifications that have been found in the plugin search paths.
    This list is valid directly after the discoverPlugins() call.
    The plugin specifications contain the information from the plugins' description files
    and the current state of the plugins. If a plugin's library has been already successfully loaded,
    the plugin specification has a reference to the created plugin instance as well.

    \sa setPluginPaths()
*/
QList<PluginSpec *> PluginManager::plugins()
{
    return m_pluginSpecs;
}

/*!
 * Returns the list of all plugin specifications that have been found in the plugin search paths,
 * grouped by category name.
 * Same remarks apply as with plugins().
 *
 * \sa plugins()
 */
QHash<QString, PluginCollection *> PluginManager::pluginCollections()
{
    return m_pluginCollections;
}

/*!
    Returns true if any plugin has errors even though it is enabled.
    Most useful to call after loadPlugins().
*/
bool PluginManager::hasError()
{
    for (PluginSpec *spec : m_pluginSpecs)
    {
        if (spec->hasError())
        {
            return true;
        }
    }
    return false;
}

/*!
    Returns all plugins that require \a spec to be loaded. Recurses into dependencies.
 */
QSet<PluginSpec *> PluginManager::pluginsRequiringPlugin(PluginSpec *spec)
{
    QSet<PluginSpec *> dependingPlugins;
    dependingPlugins.insert(spec);
    for (PluginSpec *checkSpec : loadQueue())
    {
        QHashIterator<PluginDependency, PluginSpec *> depIt(checkSpec->dependencySpecs());
        while (depIt.hasNext())
        {
            depIt.next();
            if (depIt.key().type != PluginDependency::Required)
            {
                continue;
            }
            if (dependingPlugins.contains(depIt.value()))
            {
                dependingPlugins.insert(checkSpec);
                break; // no use to check other dependencies, continue with load queue
            }
        }
    }
    dependingPlugins.remove(spec);
    return dependingPlugins;
}

/*!
    Returns all plugins that \a spec requires to be loaded. Recurses into dependencies.
 */
QSet<PluginSpec *> PluginManager::pluginsRequiredByPlugin(PluginSpec *spec)
{
    QSet<PluginSpec *> recursiveDependencies;
    recursiveDependencies.insert(spec);
    QList<PluginSpec *> queue;
    queue.append(spec);
    while (!queue.isEmpty())
    {
        PluginSpec *checkSpec = queue.takeFirst();
        QHashIterator<PluginDependency, PluginSpec *> depIt(checkSpec->dependencySpecs());
        while (depIt.hasNext())
        {
            depIt.next();
            if (depIt.key().type != PluginDependency::Required)
            {
                continue;
            }
            PluginSpec *depSpec = depIt.value();
            if (!recursiveDependencies.contains(depSpec))
            {
                recursiveDependencies.insert(depSpec);
                queue.append(depSpec);
            }
        }
    }
    recursiveDependencies.remove(spec);
    return recursiveDependencies;
}

/*!
    Sets \a settings as the user specific settings to use for information about enabled and
    disabled plugins.
    Needs to be set before calling discoverPlugins().
*/
void PluginManager::setSettings(QSettings *settings)
{
    if (m_settings)
    {
        delete m_settings;
    }
    m_settings = settings;
    if (m_settings)
    {
        m_settings->setParent(instance());
    }
}

/*!
    Returns the user specific settings used for information about enabled and
    disabled plugins.
*/
QSettings *PluginManager::settings()
{
    return m_settings;
}

/*!
    Sets \a settings as the global (user-independent) settings to use for information about
    default disabled plugins.
    Needs to be set before calling discoverPlugins().
*/
void PluginManager::setGlobalSettings(QSettings *settings)
{
    if (m_globalSettings)
    {
        delete m_globalSettings;
    }
    m_globalSettings = settings;
    if (m_globalSettings)
    {
        m_globalSettings->setParent(instance());
    }

}

/*!
    Returns the global (user-independent) settings used for information about default disabled plugins.
*/
QSettings *PluginManager::globalSettings()
{
    return m_globalSettings;
}

/*!
    Write current global and user settings to disk.
 */
void PluginManager::writeSettings()
{
    if (m_settings == nullptr)
    {
        return;
    }
    QStringList tempDisabledPlugins;
    QStringList tempForceEnabledPlugins;
    for (PluginSpec *spec : m_pluginSpecs)
    {
        if (spec->isEnabledByDefault() && !spec->isEnabledBySettings())
        {
            tempDisabledPlugins.append(spec->name());
        }
        if (!spec->isEnabledByDefault() && spec->isEnabledBySettings())
        {
            tempForceEnabledPlugins.append(spec->name());
        }
    }

    m_settings->setValue(S_IGNORED_PLUGINS, tempDisabledPlugins);
    m_settings->setValue(S_FORCEENABLED_PLUGINS, tempForceEnabledPlugins);
}

/*!
    \internal
 */
void PluginManager::readSettings()
{

    if (m_globalSettings != nullptr)
    {
        m_defaultDisabledPlugins = m_globalSettings->value(S_IGNORED_PLUGINS).toStringList();
        m_defaultEnabledPlugins = m_globalSettings->value(S_FORCEENABLED_PLUGINS).toStringList();
    }
    if (m_settings != nullptr)
    {
        m_disabledPlugins = m_settings->value(S_IGNORED_PLUGINS).toStringList();
        m_forceEnabledPlugins = m_settings->value(S_FORCEENABLED_PLUGINS).toStringList();
    }
}
