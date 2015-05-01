#include "pluginspec.h"
#include "extension.h"
#include "iplugin.h"
#include "pluginmanager.h"
#include "json.h"

#include <QRegExp>
#include <QFileInfo>

static const QString PLUGIN_METADATA      = QStringLiteral("MetaData");
static const QString PLUGIN_NAME          = QStringLiteral("Name");
static const QString PLUGIN_VERSION       = QStringLiteral("Version");
static const QString PLUGIN_COMPATVERSION = QStringLiteral("CompatVersion");
static const QString VENDOR               = QStringLiteral("Vendor");
static const QString COPYRIGHT            = QStringLiteral("Copyright");
static const QString LICENSE              = QStringLiteral("License");
static const QString DESCRIPTION          = QStringLiteral("Description");
static const QString URL                  = QStringLiteral("Url");
static const QString CATEGORY             = QStringLiteral("Category");
static const QString DEPENDENCIES         = QStringLiteral("Dependencies");

static QRegExp VERSION_REGEXP = QRegExp(QStringLiteral("([0-9]+)(?:[.]([0-9]+))?(?:[.]([0-9]+))?(?:_([0-9]+))?"));

bool isValidVersion(const QString &version)
{
    return VERSION_REGEXP.exactMatch(version);
}

int versionCompare(const QString &version1, const QString &version2)
{
    QRegExp reg1 = VERSION_REGEXP;
    QRegExp reg2 = VERSION_REGEXP;
    if (!reg1.exactMatch(version1))
        return 0;
    if (!reg2.exactMatch(version2))
        return 0;
    int number1;
    int number2;
    for (int i = 0; i < 4; ++i) {
        number1 = reg1.cap(i+1).toInt();
        number2 = reg2.cap(i+1).toInt();
        if (number1 < number2)
            return -1;
        if (number1 > number2)
            return 1;
    }
    return 0;
}

uint qHash(const PluginDependency &value)
{
    return qHash(value.name);
}

bool PluginDependency::operator==(const PluginDependency &other) const
{
    return name == other.name && version == other.version;
}

PluginSpec::PluginSpec():
    m_plugin(nullptr),
    m_state(PluginSpec::Invalid)
{

}

PluginSpec::~PluginSpec()
{
    delete(m_plugin);
}

QString PluginSpec::name() const
{
    return m_name;
}

QString PluginSpec::version() const
{
    return m_version;
}

QString PluginSpec::compatVersion() const
{
    return m_compatVersion;
}

QString PluginSpec::vendor() const
{
    return m_vendor;
}

QString PluginSpec::copyright() const
{
    return m_copyright;
}

QString PluginSpec::license() const
{
    return m_license;
}

QString PluginSpec::description() const
{
    return m_description;
}

QString PluginSpec::url() const
{
    return m_url;
}

QString PluginSpec::category() const
{
    return m_category;
}

QVector<PluginDependency> PluginSpec::dependencies() const
{
    return m_dependencies;
}

QString PluginSpec::location() const
{
    return m_location;
}

QString PluginSpec::filePath() const
{
    return m_filePath;
}

bool PluginSpec::provides(const QString &pluginName, const QString &pluginVersion) const
{
    if (QString::compare(pluginName,m_name, Qt::CaseInsensitive) != 0)
        return false;
    return (versionCompare(m_version, pluginVersion) >= 0) && (versionCompare(m_compatVersion, pluginVersion) <= 0);
}

QHash<PluginDependency, PluginSpec *> PluginSpec::dependencySpecs() const
{
    return m_dependencySpecs;
}

IPlugin *PluginSpec::plugin() const
{
    return m_plugin;
}

PluginSpec::State PluginSpec::state() const
{
    return m_state;
}

bool PluginSpec::hasError() const
{
    return !m_errorString.isEmpty();
}

QString PluginSpec::errorString() const
{
    return m_errorString;
}

bool PluginSpec::read(const QString &fileName)
{
    m_name
        = m_version
        = m_compatVersion
        = m_vendor
        = m_copyright
        = m_license
        = m_description
        = m_url
        = m_category
        = m_location
        = m_filePath
        = QString();
    m_state = PluginSpec::Invalid;
    m_errorString.clear();
    m_dependencies.clear();
    m_dependencySpecs.clear();
    QFileInfo fileInfo(fileName);
    ExtensionDebug() << "Reading" << fileInfo.fileName();
    m_location = fileInfo.absolutePath();
    m_filePath = fileInfo.absoluteFilePath();
    m_loader.setFileName(m_filePath);
    if (m_loader.fileName().isEmpty()) {
        ExtensionDebug() << "Cannot open file" << m_filePath;
        return false;
    }

    if (!readMetaData(m_loader.metaData()))
        return false;

    m_state = PluginSpec::Read;
    return true;
}

bool PluginSpec::resolveDependencies(const QList<PluginSpec *> &specs)
{
    if (hasError())
        return false;
    Q_ASSERT(m_state == PluginSpec::Resolved || m_state == PluginSpec::Read);

    if (m_state == PluginSpec::Resolved)
        m_state = PluginSpec::Read;

    QHash<PluginDependency, PluginSpec *> resolvedDependencies;
    foreach (const PluginDependency &dependency, m_dependencies) {
        PluginSpec *found = 0;

        foreach (PluginSpec *spec, specs) {
            if (spec->provides(dependency.name, dependency.version)) {
                found = spec;
                break;
            }
        }
        if (!found)
            return reportError(QString("Could not resolve dependency: %1 v%2")
                               .arg(dependency.name).arg(dependency.version));
        resolvedDependencies.insert(dependency, found);
    }
    if (hasError())
        return false;

    m_dependencySpecs = resolvedDependencies;

    m_state = PluginSpec::Resolved;

    return true;
}

bool PluginSpec::load()
{
    if (hasError())
        return false;
    if (m_state == PluginSpec::Loaded)
        return true;
    Q_ASSERT(m_state == PluginSpec::Resolved);
    Q_ASSERT(m_plugin == nullptr);

    if (!m_loader.load())
        return reportError(QString("Unable to load: %1").arg(m_loader.errorString()));

    m_plugin = qobject_cast<IPlugin*>(m_loader.instance());
    if (m_plugin == nullptr)
        return reportError("Plugin doesn't implment IPlugin");

    m_state = PluginSpec::Loaded;
    return true;
}

bool PluginSpec::initialise()
{
    if (hasError())
        return false;
    if (m_state == PluginSpec::Initialized)
        return true;
    Q_ASSERT(m_state == PluginSpec::Loaded);
    Q_ASSERT(m_plugin != nullptr);

    QString error;
    if (!m_plugin->initialize(&error))
        return reportError(QString("Initialisation failed: %1").arg(error));

    m_state = PluginSpec::Initialized;
    return true;
}

bool PluginSpec::extensionsInitialised()
{
    if (hasError())
        return false;
    if (m_state == PluginSpec::Running)
        return true;
    Q_ASSERT(m_state == PluginSpec::Initialized);
    Q_ASSERT(m_plugin != nullptr);

    m_plugin->extensionsInitialized();
    m_state = PluginSpec::Running;
    return true;
}

void PluginSpec::stop()
{

}

bool PluginSpec::readMetaData(const QJsonObject &metaData)
{
    QJsonValue value;
    QString iid;

    if (!Json::toString(&m_errorString, metaData.value(QStringLiteral("IID")), iid))
        return false;
    if (iid != PluginManager::pluginIID())
        return reportError("Plugin ignored (wrong IID)");

    value = metaData.value(PLUGIN_METADATA);
    if (!value.isObject())
        return reportError("Plugin meta data not found");
    QJsonObject pluginInfo = value.toObject();

    value = pluginInfo.value(PLUGIN_NAME);
    if (!Json::toString(&m_errorString, value, m_name))
        return false;

    value = pluginInfo.value(PLUGIN_VERSION);
    if (!Json::toString(&m_errorString, value, m_version))
        return false;
    if (!isValidVersion(m_version))
        return reportError("Invalid version");

    value = pluginInfo.value(PLUGIN_COMPATVERSION);
    if (!Json::toString(&m_errorString, value, m_compatVersion))
        return false;
    if (!isValidVersion(m_version))
        return reportError("Invalid compat version");

    value = pluginInfo.value(VENDOR);
    if (!Json::toString(&m_errorString, value, m_vendor))
        return false;

    value = pluginInfo.value(COPYRIGHT);
    if (!Json::toString(&m_errorString, value, m_copyright))
        return false;

    value = pluginInfo.value(DESCRIPTION);
    if (!Json::toString(&m_errorString, value, m_description))
        return false;

    value = pluginInfo.value(URL);
    if (!Json::toString(&m_errorString, value, m_url))
        return false;

    value = pluginInfo.value(CATEGORY);
    if (!Json::toString(&m_errorString, value, m_category))
        return false;

    value = pluginInfo.value(LICENSE);
    if (!Json::toString(&m_errorString, value, m_license))
        return false;

    value = pluginInfo.value(DEPENDENCIES);
    if (!value.isUndefined() && !value.isArray())
        return reportError("Dependencies is not an array.");
    if (!value.isUndefined()) {
        QJsonArray array = value.toArray();
        foreach (QJsonValue depVal, array) {
            if (!depVal.isObject()) {
                return reportError("Dependency item is not an object.");
            }
            QJsonObject depObj = depVal.toObject();
            PluginDependency dep;
            value = depObj.value(PLUGIN_NAME);
            if (!Json::toString(&m_errorString, value, dep.name))
                return false;
            value = depObj.value(PLUGIN_VERSION);
            if (!Json::toString(&m_errorString, value, dep.version))
                return false;
            if (!isValidVersion(dep.version))
                return reportError("Invalid dependency version");
            m_dependencies.append(dep);
        }
    }

    ExtensionDebug() << "Plugin" << m_name << "version" << m_version << "/" << m_compatVersion;
    return true;
}

bool PluginSpec::reportError(const QString &message)
{
    m_errorString = message;
    return false;
}
