#include "schsettingspage.h"
#include "widget/settingswidget.h"

SchSettingsPage::SchSettingsPage(QObject *parent):
    ISettingsPage(parent),
    m_settingsWidget(new SettingsWidget)
{
    setPageName("General");
    setCategoryName("Schematics");
    setCategoryIcon(QIcon(":/icons/sch.png"));
}

QWidget *SchSettingsPage::widget()
{
    // TODO: init widget from settings
    return m_settingsWidget;
}

void SchSettingsPage::apply()
{
    // TODO: save settings
}

void SchSettingsPage::finish()
{
    //
}
