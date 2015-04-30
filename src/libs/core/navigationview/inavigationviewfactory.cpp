#include "inavigationviewfactory.h"

INavigationViewFactory::INavigationViewFactory(QObject *parent) :
    QObject(parent),
    m_priority(0)
{

}

INavigationViewFactory::~INavigationViewFactory()
{

}

void INavigationViewFactory::setDisplayName(const QString &displayName)
{
    m_displayName = displayName;
}

void INavigationViewFactory::setPriority(int priority)
{
    m_priority = priority;
}

void INavigationViewFactory::setId(QString id)
{
    m_id = id;
}

void INavigationViewFactory::setActivationSequence(const QKeySequence &keys)
{
    m_activationSequence = keys;
}

QString INavigationViewFactory::displayName() const
{
    return m_displayName;
}

int INavigationViewFactory::priority() const
{
    return m_priority;
}

QString INavigationViewFactory::id() const
{
    return m_id;
}

void INavigationViewFactory::saveSettings(int /*position*/, QWidget */*widget*/)
{

}

void INavigationViewFactory::restoreSettings(int /*position*/, QWidget */*widget*/)
{

}

