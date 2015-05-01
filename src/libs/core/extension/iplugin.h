#ifndef IPLUGIN_H
#define IPLUGIN_H

#include <QObject>
#include <QtPlugin>

class PluginSpec;

class IPlugin : public QObject
{
    Q_OBJECT
public:
    explicit IPlugin(QObject *parent = 0);
    virtual ~IPlugin();

    virtual bool initialize(QString *errorString) = 0;
    virtual void extensionsInitialized() = 0;

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
