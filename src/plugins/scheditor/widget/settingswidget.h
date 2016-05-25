#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QWidget>
#include <QMap>
#include "palette.h"

namespace SymbolEditor
{

    namespace Ui {
        class SettingsWidget;
    }

    class SettingsWidget : public QWidget
    {
        Q_OBJECT

    public:
        explicit SettingsWidget(QWidget *parent = nullptr);
        ~SettingsWidget();

    public slots:
        void loadSettings();
        void saveSettings();

    private slots:
        void updateViewScrollBars();
        void updateGuiScrollBars();
        void updateViewMouseCursor();
        void updateGuiMouseCursor();
        void updateViewOriginMark();
        void updateGuiOriginMark();

    private:
        Ui::SettingsWidget *m_ui;
        QMap<Palette::Mode, int> m_colorSchemeToIndex;
    };

}

#endif // SETTINGSWIDGET_H
