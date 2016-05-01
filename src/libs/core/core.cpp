#include "core.h"

#include <QSettings>
#include <QCoreApplication>

QSettings *Core::m_settings;
Core *Core::m_instance;

Core *Core::instance()
{
    if (m_instance == nullptr)
    {
        m_instance = new Core();
    }
    return m_instance;
}

QSettings *Core::settings()
{
    return instance()->m_settings;
}

Core::Core()
{
    m_settings =  new QSettings(QSettings::IniFormat, QSettings::UserScope,
                                QCoreApplication::organizationName(),
                                QCoreApplication::applicationName());
}
