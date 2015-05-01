#ifndef PLUGINSPEC_H
#define PLUGINSPEC_H

#include <QString>
#include <QHash>
#include <QVector>
#include <QPluginLoader>

class IPlugin;
class PluginManager;

struct PluginDependency
{
    PluginDependency() {}

    QString name;
    QString version;

    bool operator==(const PluginDependency &other) const;
};

uint qHash(const PluginDependency &value);

class PluginSpec
{
public:
    enum State { Invalid, Read, Resolved, Loaded, Initialized, Running, Stopped, Deleted};

    ~PluginSpec();

    // Informations valid after 'Read' state reached
    QString name() const;
    QString version() const;
    QString compatVersion() const;
    QString vendor() const;
    QString copyright() const;
    QString license() const;
    QString description() const;
    QString url() const;
    QString category() const;
    QVector<PluginDependency> dependencies() const;

    // other information, valid after 'Read' state is reached
    QString location() const;
    QString filePath() const;

    bool provides(const QString &pluginName, const QString &pluginVersion) const;

    // dependency specs, valid after 'Resolved' state is reached
    QHash<PluginDependency, PluginSpec *> dependencySpecs() const;

    // linked plugin instance, valid after 'Loaded' state is reached
    IPlugin *plugin() const;

    // state
    State state() const;
    bool hasError() const;
    QString errorString() const;

    bool read(const QString &fileName);
    bool resolveDependencies(const QList<PluginSpec *> &specs);
    bool load();
    bool initialise();
    bool extensionsInitialised();
    void stop();

private:
    bool readMetaData(const QJsonObject &metaData);
    bool reportError(const QString &message);

    friend class PluginManager;
    PluginSpec();
    QPluginLoader m_loader;
    QString m_name;
    QString m_version;
    QString m_compatVersion;
    QString m_vendor;
    QString m_copyright;
    QString m_license;
    QString m_description;
    QString m_url;
    QString m_category;
    QVector<PluginDependency> m_dependencies;
    QHash<PluginDependency, PluginSpec *> m_dependencySpecs;
    QString m_location;
    QString m_filePath;
    IPlugin *m_plugin;
    PluginSpec::State m_state;
    QString m_errorString;

};

#endif // PLUGINSPEC_H
