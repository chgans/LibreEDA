#include "isettingspage.h"

ISettingsPage::ISettingsPage(QObject *parent) : QObject(parent)
{

}

QString ISettingsPage::pageName() const
{
    return m_pageName;
}

QString ISettingsPage::categoryName() const
{
    return m_categoryName;
}

QIcon ISettingsPage::categoryIcon() const
{
    return m_categoryIcon;
}

void ISettingsPage::setPageName(const QString &name)
{
    m_pageName = name;
}

void ISettingsPage::setCategoryName(const QString &name)
{
    m_categoryName = name;
}

void ISettingsPage::setCategoryIcon(const QIcon &icon)
{
    m_categoryIcon = icon;
}

