#include "designlayermanager.h"
#include "designlayerset.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QStringList>
#include <QSettings>

#include <QDebug>

DesignLayerManager *DesignLayerManager::m_instance = nullptr;

DesignLayerManager::DesignLayerManager(QObject *parent):
    QObject(parent)
{

}

DesignLayerManager::~DesignLayerManager()
{

}

DesignLayerManager *DesignLayerManager::instance()
{
    if (m_instance == nullptr)
    {
        m_instance = new DesignLayerManager();
    }

    return m_instance;
}

void DesignLayerManager::setSystemPath(const QString &path)
{
    if (path == m_systemPath)
    {
        return;
    }
    m_systemPath = path;
    emit systemPathChanged(m_systemPath);
}

QString DesignLayerManager::systemPath() const
{
    return m_systemPath;
}

void DesignLayerManager::setUserPath(const QString &path)
{
    if (path == m_userPath)
    {
        return;
    }
    m_userPath = path;
    emit userPathChanged(m_userPath);
}

QString DesignLayerManager::userPath() const
{
    return m_userPath;
}

void DesignLayerManager::loadLayerSets()
{
    qDebug() << "Layerset manager: Loading layersets";
    m_sets.clear();
    for (DesignLayerSet *set : m_sets)
    {
        emit layerSetRemoved(set);
    }
    qDeleteAll(m_sets);
    m_sets << loadLayerSets(m_systemPath)
           << loadLayerSets(m_userPath);
    for (DesignLayerSet *set : m_sets)
    {
        emit layerSetAdded(set);
    }
    qDebug() << "Layerset manager: Loaded " << count() << "layersets";
}

QList<DesignLayerSet *> DesignLayerManager::loadLayerSets(const QString &path)
{
    QDir dir(path);
    if (path.isEmpty() || !dir.exists() || !dir.isReadable())
    {
        qWarning() << "Layerset manager: Skipping" << path << "=>" << dir.canonicalPath();
        return QList<DesignLayerSet *>();
    }
    qDebug() << "Layerset manager: Scanning" << path << "=>" << dir.canonicalPath();
    QStringList filters;
    filters << "*.LedaPcbLayerSet";
    QList<DesignLayerSet *> sets;
    for (QFileInfo fileInfo : dir.entryInfoList(filters))
    {
        DesignLayerSet *set = new DesignLayerSet;
        sets.append(set);
        QString id = fileInfo.baseName();
        QFile file(fileInfo.filePath());
        if (!file.open(QIODevice::ReadOnly))
        {
            continue;
        }
        file.close();
        QSettings settings(fileInfo.filePath(), QSettings::IniFormat);
        set->loadFromSettings(settings);
        qDebug() << "Layerset manager: Adding" << id << "(" << set->layers().count() << "layers )";
    }
    return sets;
}

int DesignLayerManager::count() const
{
    return m_sets.count();
}

QList<DesignLayerSet *> DesignLayerManager::layerSets() const
{
    return m_sets;
}

void DesignLayerManager::add(DesignLayerSet *set)
{
    add(QList<DesignLayerSet *>() << set);
}

void DesignLayerManager::add(QList<DesignLayerSet *> sets)
{
    for (DesignLayerSet *set : sets)
    {
        if (!m_sets.contains(set))
        {
            m_sets.append(set);
            emit layerSetAdded(set);
        }
    }
}

void DesignLayerManager::remove(DesignLayerSet *set)
{
    remove(QList<DesignLayerSet *>() << set);
}

void DesignLayerManager::remove(QList<DesignLayerSet *> sets)
{
    for (DesignLayerSet *set : sets)
    {
        if (m_sets.contains(set))
        {
            m_sets.removeOne(set);
            emit layerSetRemoved(set);
        }
    }
}
