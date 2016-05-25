#pragma once

#include "core/settings/isettingspage.h"

namespace SymbolEditor
{

    class SettingsWidget;

    class AppearanceSettingsPage : public ISettingsPage
    {
        Q_OBJECT

    public:
        explicit AppearanceSettingsPage(QObject *parent = nullptr);

    signals:
        void settingsChanged();

    private:
        SettingsWidget *m_widget;

        // ISettingsPage interface
    public:
        QWidget *widget();
        void apply();
        void finish();
    };

}
