#include "appearancesettingspage.h"
#include "widget/settingswidget.h"

// TODO:
//  - Add page configuring snap behaviour
//  - Add page for item and tool 'task widget' (property editors)
//    - Allow to choose b/w QtAbstractPropertyBrowser (button, group box, tree)

using namespace SymbolEditor;

AppearanceSettingsPage::AppearanceSettingsPage(QObject *parent):
    ISettingsPage(parent), m_widget(nullptr)
{
    setPageName("Appearance");
    setCategoryName("Schematics");
    setCategoryIcon(QIcon(":/icons/sch.png"));
}

QWidget *AppearanceSettingsPage::widget()
{
    m_widget = new SettingsWidget;
    m_widget->loadSettings();
    return m_widget;
}

void AppearanceSettingsPage::apply()
{
    m_widget->saveSettings();
    emit settingsChanged();
}

void AppearanceSettingsPage::finish()
{
    //
}
