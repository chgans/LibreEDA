#ifndef EXTENSION_H
#define EXTENSION_H

#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(ExtensionLoggingCategory)

#define ExtensionDebug(...) qCDebug(ExtensionLoggingCategory, __VA_ARGS__)
#define ExtensionWarning(...) qCWarning(ExtensionLoggingCategory, __VA_ARGS__)
#define ExtensionCritial(...) qCCritical(ExtensionLoggingCategory, __VA_ARGS__)

#endif // EXTENSION_H

