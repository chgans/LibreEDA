#include "iplugin.h"
#include "pluginmanager.h"

IPlugin::IPlugin(QObject *parent) : QObject(parent)
{

}

IPlugin::~IPlugin()
{
    foreach (QObject *obj, m_addedObjectsInReverseOrder)
        PluginManager::removeObject(obj);
    qDeleteAll(m_addedObjectsInReverseOrder);
    m_addedObjectsInReverseOrder.clear();
}

PluginSpec *IPlugin::pluginSpec() const
{
    return m_pluginSpec;
}

void IPlugin::addObject(QObject *obj)
{
    PluginManager::addObject(obj);
}

void IPlugin::addAutoReleasedObject(QObject *obj)
{
    m_addedObjectsInReverseOrder.prepend(obj);
}

void IPlugin::removeObject(QObject *obj)
{
    PluginManager::removeObject(obj);
}

