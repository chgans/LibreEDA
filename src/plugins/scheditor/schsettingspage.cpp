#include "schsettingspage.h"
#include "widget/settingswidget.h"

SchSettingsPage::SchSettingsPage(QObject *parent):
    ISettingsPage(parent), m_widget(nullptr)
{
    setPageName("Appearance");
    setCategoryName("Schematics");
    setCategoryIcon(QIcon(":/icons/sch.png"));
}

QWidget *SchSettingsPage::widget()
{
    m_widget = new SettingsWidget;
    m_widget->loadSettings();
    return m_widget;
}

void SchSettingsPage::apply()
{
    m_widget->saveSettings();
    emit settingsChanged();
}

void SchSettingsPage::finish()
{
    //
}
