#include "pluginspec.h"
#include "extension.h"
#include "iplugin.h"
#include "pluginmanager.h"
#include "json.h"

#include <QRegExp>
#include <QFileInfo>

/*!
    \class PluginDependency
    \inmodule LibreEDA
    \ingroup LeExtensionSystem
    \preliminary
    \brief The PluginDependency class contains the name and required compatible
    version number of a plugin's dependency.

    This reflects the data of a dependency object in the plugin's meta data.
    The name and version are used to resolve the dependency. That is,
    a plugin with the given name and
    plugin \c {compatibility version <= dependency version <= plugin version} is searched for.

    See also ExtensionSystem::IPlugin for more information about plugin dependencies and
    version matching.
*/

/*!
    \fn void PluginDependency::PluginDependency()
    \internal
*/

/*!
    \variable PluginDependency::name
    String identifier of the plugin.
*/

/*!
    \variable PluginDependency::version
    Version string that a plugin must match to fill this dependency.
*/

/*!
    \variable PluginDependency::type
    Defines whether the dependency is required or optional.
    \sa PluginDependency::Type
*/

/*!
    \enum PluginDependency::Type
    Whether the dependency is required or optional.
    \value Required
           Dependency needs to be there.
    \value Optional
           Dependency is not necessarily needed. You need to make sure that
           the plugin is able to load without this dependency installed, so
           for example you may not link to the dependency's library.
    \value Test
           Dependency needs to be force-loaded for running tests of the plugin.
*/

/*!
    \class PluginSpec
    \inmodule ExtensionSystem
    \brief The PluginSpec class contains the information of the plugin's embedded meta data
    and information about the plugin's current state.

    The plugin spec is also filled with more information as the plugin
    goes through its loading process (see PluginSpec::State).
    If an error occurs, the plugin spec is the place to look for the
    error details.
*/

/*!
    \enum PluginSpec::State
    The State enum indicates the states the plugin goes through while
    it is being loaded.

    The state gives a hint on what went wrong in case of an error.

    \value  Invalid
            Starting point: Even the plugin meta data was not read.
    \value  Read
            The plugin meta data has been successfully read, and its
            information is available via the PluginSpec.
    \value  Resolved
            The dependencies given in the description file have been
            successfully found, and are available via the dependencySpecs() function.
    \value  Loaded
            The plugin's library is loaded and the plugin instance created
            (available through plugin()).
    \value  Initialized
            The plugin instance's IPlugin::initialize() function has been called
            and returned a success value.
    \value  Running
            The plugin's dependencies are successfully initialized and
            extensionsInitialized has been called. The loading process is
            complete.
    \value Stopped
            The plugin has been shut down, i.e. the plugin's IPlugin::aboutToShutdown() function has been called.
    \value Deleted
            The plugin instance has been deleted.
*/

/*!
    \class PluginArgumentDescription
    \inmodule LibreEDA
    \ingroup LeExtensionSystem
    \preliminary
    \brief The PluginArgumentDescription class contains the name, the parameter and the
           description of a plugin's argument
 */

/*!
    \variable PluginArgumentDescription::name
    The name of the argument.
    \sa PluginDependency::Type
*/

/*!
    \variable PluginArgumentDescription::parameter
    Short description of the argument's value.
    \sa PluginDependency::Type
*/

/*!
    \variable PluginArgumentDescription::description
    The description of the argument (has to be one line only).
    \sa PluginDependency::Type
*/

static const QString PLUGIN_METADATA      = QStringLiteral("MetaData");
static const QString PLUGIN_NAME          = QStringLiteral("Name");
static const QString PLUGIN_VERSION       = QStringLiteral("Version");
static const QString PLUGIN_COMPATVERSION = QStringLiteral("CompatVersion");
static const QString PLUGIN_REQUIRED      = QStringLiteral("Required");
static const QString PLUGIN_EXPERIMENTAL  = QStringLiteral("Experimental");
static const QString PLUGIN_DISABLED_BY_DEFAULT  = QStringLiteral("DisabledByDefault");
static const QString VENDOR               = QStringLiteral("Vendor");
static const QString COPYRIGHT            = QStringLiteral("Copyright");
static const QString LICENSE              = QStringLiteral("License");
static const QString DESCRIPTION          = QStringLiteral("Description");
static const QString URL                  = QStringLiteral("Url");
static const QString CATEGORY             = QStringLiteral("Category");
static const QString DEPENDENCIES         = QStringLiteral("Dependencies");
static const QString DEPENDENCY_NAME      = QStringLiteral("Name");
static const QString DEPENDENCY_VERSION   = QStringLiteral("Version");
static const QString DEPENDENCY_TYPE      = QStringLiteral("Type");
static const QString DEPENDENCY_TYPE_SOFT = QStringLiteral("Optional");
static const QString DEPENDENCY_TYPE_HARD = QStringLiteral("Required");
static const QString DEPENDENCY_TYPE_TEST = QStringLiteral("Test");
static const QString ARGUMENTS            = QStringLiteral("Arguments");
static const QString ARGUMENT_NAME        = QStringLiteral("Name");
static const QString ARGUMENT_PARAMETER   = QStringLiteral("Parameter");
static const QString ARGUMENT_DESCRIPTION = QStringLiteral("Description");

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

/*!
    \internal
*/
bool PluginDependency::operator==(const PluginDependency &other) const
{
    return name == other.name && version == other.version && type == other.type;
}

/*!
    \internal
*/
PluginSpec::PluginSpec()
{
    clearData();
}

/*!
    \internal
*/
void PluginSpec::clearData()
{
    m_plugin= nullptr;
    m_state = PluginSpec::Invalid;
    m_isRequired = false;
    m_isExperimental = false,
    m_isDisabledByDefault = false;
    m_isEnabledBySettings = true;
    m_isEnabledIndirectly = false;
    m_isForceEnabled = false;
    m_isForceDisabled = false;
    m_hasError = false;
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
    m_errorString.clear();
    m_dependencies.clear();
    m_dependencySpecs.clear();
    m_argumentDescriptions.clear();
    m_arguments.clear();
}

/*!
    \internal
*/
PluginSpec::~PluginSpec()
{
    delete(m_plugin);
}

/*!
    The plugin name. This is valid after the PluginSpec::Read state is reached.
*/
QString PluginSpec::name() const
{
    return m_name;
}

/*!
    The plugin version. This is valid after the PluginSpec::Read state is reached.
*/
QString PluginSpec::version() const
{
    return m_version;
}

/*!
    The plugin compatibility version. This is valid after the PluginSpec::Read state is reached.
*/
QString PluginSpec::compatVersion() const
{
    return m_compatVersion;
}

/*!
    The plugin vendor. This is valid after the PluginSpec::Read state is reached.
*/
QString PluginSpec::vendor() const
{
    return m_vendor;
}

/*!
    The plugin copyright. This is valid after the PluginSpec::Read state is reached.
*/
QString PluginSpec::copyright() const
{
    return m_copyright;
}

/*!
    The plugin license. This is valid after the PluginSpec::Read state is reached.
*/
QString PluginSpec::license() const
{
    return m_license;
}

/*!
    The plugin description. This is valid after the PluginSpec::Read state is reached.
*/
QString PluginSpec::description() const
{
    return m_description;
}

/*!
    The plugin URL where you can find more information about the plugin.
    This is valid after the PluginSpec::Read state is reached.
*/
QString PluginSpec::url() const
{
    return m_url;
}

/*!
    The category that the plugin belongs to. Categories are groups of plugins which
    allow for keeping them together in the UI.
    Returns an empty string if the plugin does not belong to a category.
*/
QString PluginSpec::category() const
{
    return m_category;
}

/*!
    Returns whether the plugin has its required flag set.
*/
bool PluginSpec::isRequired() const
{
    return m_isRequired;
}

/*!
    Returns whether the plugin has its experimental flag set.
*/
bool PluginSpec::isExperimental() const
{
    return m_isExperimental;
}

/*!
    Returns whether the plugin is enabled by default.
    A plugin might be disabled because the plugin is experimental, or because
    the install settings define it as disabled by default.
*/
bool PluginSpec::isEnabledByDefault() const
{
    return !m_isDisabledByDefault;
}

/*!
    Returns whether the plugin should be loaded at startup,
    taking into account the default enabled state, and the user's settings.

    \note This function might return false even if the plugin is loaded as a requirement of another
    enabled plugin.
    \sa PluginSpec::isEffectivelyEnabled
*/
bool PluginSpec::isEnabledBySettings() const
{
    return m_isEnabledBySettings;
}

/*!
    Returns whether the plugin is loaded at startup.
    \sa PluginSpec::isEnabledBySettings
*/
bool PluginSpec::isEffectivelyEnabled() const
{
    if (isForceEnabled() || isEnabledIndirectly())
        return true;
    if (isForceDisabled())
        return false;
    return isEnabledBySettings();
}

/*!
    Returns true if loading was not done due to user unselecting this plugin or its dependencies.
*/
bool PluginSpec::isEnabledIndirectly() const
{
    return m_isEnabledIndirectly;
}

/*!
    Returns whether the plugin is enabled via the -load option on the command line.
*/
bool PluginSpec::isForceEnabled() const
{
    return m_isForceEnabled;
}

/*!
    Returns whether the plugin is disabled via the -noload option on the command line.
*/
bool PluginSpec::isForceDisabled() const
{
    return m_isForceDisabled;
}

/*!
    The plugin dependencies. This is valid after the PluginSpec::Read state is reached.
*/
QVector<PluginDependency> PluginSpec::dependencies() const
{
    return m_dependencies;
}

/*!
    Returns a list of descriptions of command line arguments the plugin processes.
*/
PluginSpec::PluginArgumentDescriptions PluginSpec::argumentDescriptions() const
{
    return m_argumentDescriptions;
}

/*!
    The absolute path to the directory containing the plugin description file
    this PluginSpec corresponds to.
*/
QString PluginSpec::location() const
{
    return m_location;
}

/*!
    The absolute path to the plugin description file (including the file name)
    this PluginSpec corresponds to.
*/
QString PluginSpec::filePath() const
{
    return m_filePath;
}

/*!
    Command line arguments specific to the plugin. Set at startup.
*/
QStringList PluginSpec::arguments() const
{
    return m_arguments;
}

/*!
    Sets the command line arguments specific to the plugin to \a arguments.
*/
void PluginSpec::setArguments(const QStringList &arguments)
{
    m_arguments = arguments;
}

/*!
    Adds \a argument to the command line arguments specific to the plugin.
*/
void PluginSpec::addArgument(const QString &argument)
{
    m_arguments.push_back(argument);
}

/*!
    Returns whether this plugin can be used to fill in a dependency of the given
    \a pluginName and \a pluginVersion.

    \sa dependencies()
*/
bool PluginSpec::provides(const QString &pluginName, const QString &pluginVersion) const
{
    if (QString::compare(pluginName,m_name, Qt::CaseInsensitive) != 0)
        return false;
    return (versionCompare(m_version, pluginVersion) >= 0) && (versionCompare(m_compatVersion, pluginVersion) <= 0);
}

/*!
    Returns the list of dependencies, already resolved to existing plugin specs.
    Valid if PluginSpec::Resolved state is reached.

    \sa dependencies()
*/
QHash<PluginDependency, PluginSpec *> PluginSpec::dependencySpecs() const
{
    return m_dependencySpecs;
}

/*!
    The corresponding IPlugin instance, if the plugin library has already been successfully loaded,
    i.e. the PluginSpec::Loaded state is reached.
*/
IPlugin *PluginSpec::plugin() const
{
    return m_plugin;
}

/*!
    The state in which the plugin currently is.
    See the description of the PluginSpec::State enum for details.
*/
PluginSpec::State PluginSpec::state() const
{
    return m_state;
}

/*!
    Returns whether an error occurred while reading/starting the plugin.

    \sa errorString()
*/
bool PluginSpec::hasError() const
{
    return m_hasError;
}

/*!
    Detailed, possibly multi-line, error description in case of an error.
*/
QString PluginSpec::errorString() const
{
    return m_errorString;
}

/*!
    \internal
*/
bool PluginSpec::read(const QString &fileName)
{
    clearData();
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

/*!
    \internal
*/
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

/*!
    \internal
*/
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

/*!
    \internal
*/
bool PluginSpec::initialise()
{
    if (hasError())
        return false;
    if (m_state == PluginSpec::Initialized)
        return true;
    Q_ASSERT(m_state == PluginSpec::Loaded);
    Q_ASSERT(m_plugin != nullptr);

    QString error;
    if (!m_plugin->initialize(m_arguments, &error))
        return reportError(QString("Initialisation failed: %1").arg(error));

    m_state = PluginSpec::Initialized;
    return true;
}

/*!
    \internal
*/
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

/*!
    \internal
*/
void PluginSpec::stop()
{

}

/*!
    \internal
*/
void PluginSpec::setEnabledBySettings(bool value)
{
    m_isEnabledBySettings = value;
}

/*!
    \internal
*/
void PluginSpec::setEnabledByDefault(bool value)
{
    m_isDisabledByDefault = !value;
}

/*!
    \internal
*/
void PluginSpec::setForceEnabled(bool value)
{
    m_isForceEnabled = value;
    if (value)
        m_isForceDisabled = false;
}

/*!
    \internal
*/
void PluginSpec::setForceDisabled(bool value)
{
    if (value)
        m_isForceEnabled = false;
    m_isForceDisabled = value;
}

/*!
    \internal
*/
// FIXME: always use reportError(), since it sets the error flag.
// TODO: Better error reporting
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
    m_compatVersion = m_version;
    if (!value.isUndefined()) {
        if (!Json::toString(&m_errorString, value, m_compatVersion))
            return false;
        if (!isValidVersion(m_version))
            return reportError("Invalid compat version");
    }

    value = pluginInfo.value(PLUGIN_REQUIRED);
    if (!value.isUndefined() && !Json::toBool(&m_errorString, value, m_isRequired))
        return false;

    value = pluginInfo.value(PLUGIN_EXPERIMENTAL);
    if (!value.isUndefined() && !Json::toBool(&m_errorString, value, m_isExperimental))
        return false;

    value = pluginInfo.value(PLUGIN_DISABLED_BY_DEFAULT);
    if (!value.isUndefined() && !Json::toBool(&m_errorString, value, m_isDisabledByDefault))
        return false;

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
            value = depObj.value(DEPENDENCY_NAME);
            if (!Json::toString(&m_errorString, value, dep.name))
                return false;
            value = depObj.value(DEPENDENCY_VERSION);
            if (!Json::toString(&m_errorString, value, dep.version))
                return false;
            if (!isValidVersion(dep.version))
                return reportError("Invalid dependency version");
            value = depObj.value(DEPENDENCY_TYPE);
            QString str = DEPENDENCY_TYPE_HARD;
            if (!value.isUndefined() && !Json::toString(&m_errorString, value, str))
                return false;
            if (str == DEPENDENCY_TYPE_HARD)
                dep.type = PluginDependency::Required;
            else if (str == DEPENDENCY_TYPE_SOFT)
                dep.type = PluginDependency::Optional;
            else if (str == DEPENDENCY_TYPE_TEST)
                dep.type = PluginDependency::Test;
            else {
                return reportError("Invalid depemdency type");
            }
            m_dependencies.append(dep);
        }
    }

    value = pluginInfo.value(ARGUMENTS);
    if (!value.isUndefined() && !value.isArray())
        return reportError("Invalid argument array");
    if (!value.isUndefined()) {
        QJsonArray array = value.toArray();
        foreach (const QJsonValue &v, array) {
            if (!v.isObject())
                return reportError("Invalid argument entry");
            QJsonObject argumentObject = v.toObject();
            PluginArgumentDescription arg;
            value = argumentObject.value(ARGUMENT_NAME);
            if (!Json::toString(&m_errorString, value, arg.name))
                return false;
            value = argumentObject.value(ARGUMENT_PARAMETER);
            if (!Json::toString(&m_errorString, argumentObject, arg.parameter))
                return false;
            value = argumentObject.value(ARGUMENT_DESCRIPTION);
            if (!Json::toString(&m_errorString, argumentObject, arg.description))
                return false;
            m_argumentDescriptions.append(arg);
        }
    }
    ExtensionDebug() << "Plugin" << m_name << "version" << m_version << "/" << m_compatVersion;
    return true;
}

/*!
    \internal
*/
bool PluginSpec::reportError(const QString &message)
{
    m_hasError = true;
    m_errorString = message;
    return false;
}
