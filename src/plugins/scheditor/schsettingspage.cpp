#include "schsettingspage.h"
#include "widget/settingswidget.h"

SchSettingsPage::SchSettingsPage(QObject *parent):
    ISettingsPage(parent)
{
    setPageName("General");
    setCategoryName("Schematics");
    setCategoryIcon(QIcon(":/icons/sch.png"));
}

QWidget *SchSettingsPage::widget()
{
    // TODO: init widget from settings
    return new SettingsWidget;
}

void SchSettingsPage::apply()
{
    // TODO: save settings
}

void SchSettingsPage::finish()
{
    //
}
