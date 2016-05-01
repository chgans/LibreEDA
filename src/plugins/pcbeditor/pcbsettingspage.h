#ifndef PCBSETTINGSPAGE_H
#define PCBSETTINGSPAGE_H

#include "core/settings/isettingspage.h"

class PcbSettingsPage: public ISettingsPage
{
    Q_OBJECT
public:
    explicit PcbSettingsPage(QObject *parent = nullptr);

    // ISettingsPage interface
public:
    QWidget *widget();
    void apply();
    void finish();
};

#endif // PCBSETTINGSPAGE_H
