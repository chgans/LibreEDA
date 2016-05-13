#include "iplugin.h"
#include "pluginmanager.h"

/*!
    \class IPlugin
    \inmodule LibreEDA
    \ingroup LeExtensionSystem
    \preliminary
    \mainclass
    \brief The IPlugin class is the base class for all plugins.

    The IPlugin class is an abstract class that must be implemented
    once for each plugin.
    A plugin consists of two parts: A description file, and a library
    that at least contains the IPlugin implementation.

    \tableofcontents

    \section1 Plugin Specification

    A plugin needs to provide a plugin specification file in addition
    to the actual plugin library, so the plugin manager can find the plugin,
    resolve its dependencies, and load it. For more information,
    see \l{Plugin Meta Data}.

    \section1 Plugin Implementation
    Plugins must provide one implementation of the IPlugin class, located
    in a library that matches the \c name attribute given in their
    description file. The IPlugin implementation must be exported and
    made known to Qt's plugin system, using the \c Q_PLUGIN_METADATA macro with
    an IID set to \c "org.libre-eda.leda.plugin".


    After the plugins' description files have been read, and dependencies have been
    found, the plugin loading is done in three phases:
    \list 1
    \li All plugin libraries are loaded in \e{root-to-leaf} order of the
       dependency tree.
    \li All plugins' initialize functions are called in \e{root-to-leaf} order
       of the dependency tree. This is a good place to put
       objects in the plugin manager's object pool.
    \li All plugins' extensionsInitialized functions are called in \e{leaf-to-root}
       order of the dependency tree. At this point, plugins can
       be sure that all plugins that depend on this plugin have
       been initialized completely (implying that they have put
       objects in the object pool, if they want that during the
       initialization sequence).
    \endlist
    If library loading or initialization of a plugin fails, all plugins
    that depend on that plugin also fail.

    Plugins have access to the plugin manager
    (and its object pool) via the PluginManager::instance()
    function.
*/

/*!
    \fn bool IPlugin::initialize(QString *errorString)
    \brief Called after the plugin has been loaded and the IPlugin instance
    has been created.

    The initialize functions of plugins that depend
    on this plugin are called after the initialize function of this plugin
    has been called. Plugins should initialize their internal state in this
    function. Returns if initialization of successful. If it wasn't successful,
    the \a errorString should be set to a user-readable message
    describing the reason.

    \sa extensionsInitialized()
*/

/*!
    \fn void IPlugin::extensionsInitialized()
    \brief Called after the IPlugin::initialize() function has been called,
    and after both the IPlugin::initialize() and IPlugin::extensionsInitialized()
    functions of plugins that depend on this plugin have been called.

    In this function, the plugin can assume that plugins that depend on
    this plugin are fully 'up and running'. It is a good place to
    look in the plugin manager's object pool for objects that have
    been provided by dependent plugins.

    \sa initialize()
*/

/*!
    \fn void IPlugin::shutdown()
    \brief Called during a shutdown sequence in the same order as initialization
    before the plugins get deleted in reverse order.

    This function should be used to disconnect from other plugins,
    hide all UI, and optimize shutdown in general.
*/

/*!
 * \internal
 */
IPlugin::IPlugin(QObject *parent) : QObject(parent)
{

}

/*!
 * \internal
 */
IPlugin::~IPlugin()
{
    for (QObject *obj : m_addedObjectsInReverseOrder)
    {
        PluginManager::removeObject(obj);
    }
    qDeleteAll(m_addedObjectsInReverseOrder);
    m_addedObjectsInReverseOrder.clear();
}

/*!
    Returns the PluginSpec corresponding to this plugin.
    This is not available in the constructor.
*/
PluginSpec *IPlugin::pluginSpec() const
{
    return m_pluginSpec;
}

/*!
    Convenience function that registers \a obj in the plugin manager's
    plugin pool by just calling PluginManager::addObject().
*/
void IPlugin::addObject(QObject *obj)
{
    PluginManager::addObject(obj);
}

/*!
    Convenience function for registering \a obj in the plugin manager's
    plugin pool. Usually, registered objects must be removed from
    the object pool and deleted by hand.
    Objects added to the pool via addAutoReleasedObject are automatically
    removed and deleted in reverse order of registration when
    the IPlugin instance is destroyed.
    \sa PluginManager::addObject()
*/
void IPlugin::addAutoReleasedObject(QObject *obj)
{
    m_addedObjectsInReverseOrder.prepend(obj);
}

/*!
    Convenience function that unregisters \a obj from the plugin manager's
    plugin pool by just calling PluginManager::removeObject().
*/
void IPlugin::removeObject(QObject *obj)
{
    PluginManager::removeObject(obj);
}

