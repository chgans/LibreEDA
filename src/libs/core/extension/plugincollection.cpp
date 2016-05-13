#include "plugincollection.h"

/*!
    \class PluginCollection
    \inmodule LibreEDA
    \ingroup LeExtensionSystem
    \preliminary
    \brief The PluginCollection class is a convenience class to logically group PluginSpec objects.
 */

/*!
 * Constructs a PluginCollection with the given \a name.
 */
PluginCollection::PluginCollection(const QString &name) :
    m_name(name)
{

}

/*!
 * Destroys the PluginCollection.
 */
PluginCollection::~PluginCollection()
{

}

/*!
 * The name of the collection.
 */
QString PluginCollection::name() const
{
    return m_name;
}

/*!
 * Adds \a spec to the collection.
 */
void PluginCollection::addPlugin(PluginSpec *spec)
{
    m_plugins.append(spec);
}

/*!
 * Removes \a spec to the collection.
 */
void PluginCollection::removePlugin(PluginSpec *spec)
{
    m_plugins.removeOne(spec);
}

/*!
 * Returns the plugins belonging to this collection.
 */
QList<PluginSpec *> PluginCollection::plugins() const
{
    return m_plugins;
}
