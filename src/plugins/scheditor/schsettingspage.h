#ifndef SCHSETTINGSPAGE_H
#define SCHSETTINGSPAGE_H

#include "core/settings/isettingspage.h"

class SchSettingsPage : public ISettingsPage
{
    Q_OBJECT

public:
    SchSettingsPage(QObject *parent);

private:
    QWidget *m_settingsWidget;

    // ISettingsPage interface
public:
    QWidget *widget();
    void apply();
    void finish();
};

#endif // SCHSETTINGSPAGE_H
