#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QWidget>
#include <QMap>
#include "palette.h"

namespace Ui {
class SettingsWidget;
}

class SettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsWidget(QWidget *parent = 0);
    ~SettingsWidget();

private slots:
    void updateViewScrollBars();
    void updateGuiScrollBars();
    void updateViewMouseCursor();
    void updateGuiMouseCursor();
    void updateViewOriginMark();
    void updateGuiOriginMark();

private:
    Ui::SettingsWidget *ui;
    QMap<Palette::Mode, int> m_colorSchemeToIndex;
};

#endif // SETTINGSWIDGET_H
