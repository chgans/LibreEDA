#ifndef IPLUGIN_H
#define IPLUGIN_H

#include "core/core_global.h"

#include <QObject>
#include <QtPlugin>

class PluginSpec;

class CORE_EXPORT IPlugin : public QObject
{
    Q_OBJECT
public:
    explicit IPlugin(QObject *parent = 0);
    virtual ~IPlugin();

    virtual bool initialize(const QStringList &arguments, QString *errorString) = 0;
    virtual void extensionsInitialized() = 0;
    virtual void shutdown() = 0;

    PluginSpec *pluginSpec() const;

    void addObject(QObject *obj);
    void addAutoReleasedObject(QObject *obj);
    void removeObject(QObject *obj);

signals:

public slots:

private:
    PluginSpec *m_pluginSpec;
    QList<QObject *> m_addedObjectsInReverseOrder;
};

#endif // IPLUGIN_H
