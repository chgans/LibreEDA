#include "ioutputpanefactory.h"

IOutputPaneFactory::IOutputPaneFactory(QObject *parent) : QObject(parent), m_priority(-1)
{

}

IOutputPaneFactory::~IOutputPaneFactory()
{

}

void IOutputPaneFactory::setDisplayName(const QString &displayName)
{
    m_displayName = displayName;
}

void IOutputPaneFactory::setPriority(int priority)
{
    m_priority = priority;
}

QString IOutputPaneFactory::displayName() const
{
    return m_displayName;
}

int IOutputPaneFactory::priority() const
{
    return m_priority;
}
