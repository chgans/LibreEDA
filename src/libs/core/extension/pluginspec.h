#ifndef PLUGINSPEC_H
#define PLUGINSPEC_H

#include "core/core_global.h"

#include <QString>
#include <QHash>
#include <QVector>
#include <QPluginLoader>
#include <QStringList>

class IPlugin;
class PluginManager;

struct PluginDependency
{
    enum Type {
        Required,
        Optional,
        Test
    };

    PluginDependency() : type(Required) {}

    QString name;
    QString version;
    Type type;

    bool operator==(const PluginDependency &other) const;
};

uint qHash(const PluginDependency &value);

struct PluginArgumentDescription
{
    QString name;
    QString parameter;
    QString description;
};

class CORE_EXPORT PluginSpec
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
    bool isRequired() const;
    bool isExperimental() const;
    bool isEnabledByDefault() const;
    bool isEnabledBySettings() const;
    bool isEffectivelyEnabled() const;
    bool isEnabledIndirectly() const;
    bool isForceEnabled() const;
    bool isForceDisabled() const;
    QVector<PluginDependency> dependencies() const;

    typedef QVector<PluginArgumentDescription> PluginArgumentDescriptions;
    PluginArgumentDescriptions argumentDescriptions() const;

    // other information, valid after 'Read' state is reached
    QString location() const;
    QString filePath() const;

    QStringList arguments() const;
    void setArguments(const QStringList &arguments);
    void addArgument(const QString &argument);

    bool provides(const QString &pluginName, const QString &pluginVersion) const;

    // dependency specs, valid after 'Resolved' state is reached
    QHash<PluginDependency, PluginSpec *> dependencySpecs() const;

    // linked plugin instance, valid after 'Loaded' state is reached
    IPlugin *plugin() const;

    // state
    State state() const;
    bool hasError() const;
    QString errorString() const;


private:
    friend class PluginManager;
    PluginSpec();
    void clearData();
    bool reportError(const QString &message);
    bool read(const QString &fileName);
    bool readMetaData(const QJsonObject &metaData);
    bool resolveDependencies(const QList<PluginSpec *> &specs);
    bool load();
    bool initialise();
    bool extensionsInitialised();
    void stop();
    void kill();
    void setEnabledBySettings(bool value);
    void setEnabledByDefault(bool value);
    void setForceEnabled(bool value);
    void setForceDisabled(bool value);

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
    PluginSpec::PluginArgumentDescriptions m_argumentDescriptions;
    QHash<PluginDependency, PluginSpec *> m_dependencySpecs;
    QStringList m_arguments;
    QString m_location;
    QString m_filePath;
    IPlugin *m_plugin;
    PluginSpec::State m_state;
    QString m_errorString;
    bool m_hasError;
    bool m_isRequired;
    bool m_isExperimental;
    bool m_isDisabledByDefault;
    bool m_isEnabledBySettings;
    bool m_isEnabledIndirectly;
    bool m_isForceEnabled;
    bool m_isForceDisabled;
};

#endif // PLUGINSPEC_H
