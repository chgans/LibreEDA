#ifndef PLUGINCOLLECTION_H
#define PLUGINCOLLECTION_H

#include "core/core_global.h"

#include <QString>
#include <QList>

class PluginSpec;

class CORE_EXPORT PluginCollection
{
public:
    explicit PluginCollection(const QString& name);
    ~PluginCollection();

    QString name() const;
    void addPlugin(PluginSpec *spec);
    void removePlugin(PluginSpec *spec);
    QList<PluginSpec *> plugins() const;
private:
    QString m_name;
    QList<PluginSpec *> m_plugins;
};

#endif // PLUGINCOLLECTION_H
