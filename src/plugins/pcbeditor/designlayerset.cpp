#include "designlayerset.h"

#include <QSettings>
#include <QDebug>

DesignLayerSet::DesignLayerSet(QObject *parent):
    QObject(parent),
    m_isSystem(false)
{

}

DesignLayerSet::~DesignLayerSet()
{

}

QString DesignLayerSet::name() const
{
    return m_name;
}

void DesignLayerSet::setName(const QString &name)
{
    if (m_name == name)
        return;
    m_name = name;
    QString ename = effectiveName();
    emit nameChanged(m_name);
    if (ename != effectiveName())
        emit effectiveNameChanged(effectiveName());
}

QString DesignLayerSet::customName() const
{
    return m_customName;
}

QString DesignLayerSet::effectiveName() const
{
    if (m_customName.isEmpty())
        return m_name;
    return m_customName;
}

void DesignLayerSet::setCustomName(const QString &name)
{
    if (m_customName == name)
        return;
    m_customName = name;
    QString ename = effectiveName();
    emit customNameChanged(m_customName);
    if (ename != effectiveName())
        emit effectiveNameChanged(effectiveName());
}

bool DesignLayerSet::isSystem() const
{
    return m_isSystem;
}

void DesignLayerSet::setIsSystem(bool isSystem)
{
    if (isSystem == m_isSystem)
        return;
    m_isSystem = isSystem;
    // emit isSystemChanged(m_isSystem);
}

QList<int> DesignLayerSet::layers()
{
    return m_layers;
}

void DesignLayerSet::loadFromSettings(QSettings &settings)
{
    settings.beginGroup("PcbLayerSet");
    setIsSystem(settings.value("system", false).toBool());
    setName(settings.value("label", "<unknown>").toString());
    int nb = settings.beginReadArray("layers");
    for (int i = 0; i < nb; i++) {
        m_layers.append(settings.value(QString("%1").arg(i), -1).toInt());
    }
    settings.endArray();
    settings.endGroup();
}

void DesignLayerSet::saveToSettings(QSettings &settings) const
{
    settings.beginGroup("PcbLayerSet");
    settings.setValue("system", isSystem());
    settings.setValue("label", name());
    settings.beginReadArray("layers");
    for (int i = 0; i < m_layers.count(); i++) {
        settings.setValue(QString("%1").arg(i), m_layers.value(i));
    }
    settings.endArray();
    settings.endGroup();
}

void DesignLayerSet::add(int index)
{
    add(QList<int>() << index);
}

void DesignLayerSet::add(const QList<int> &indexes)
{
    bool changed = false;
    for (int idx: indexes) {
        if (!m_layers.contains(idx)) {
            changed = true;
            m_layers.append(idx);
        }
    }
    if (changed)
        emit layersChanged(m_layers);
}

void DesignLayerSet::remove(int index)
{
    remove(QList<int>() << index);
}

void DesignLayerSet::remove(const QList<int> &indexes)
{
    bool changed = false;
    for (int idx: indexes)
        changed |= m_layers.removeOne(idx);
    if (changed)
        emit layersChanged(m_layers);
}

