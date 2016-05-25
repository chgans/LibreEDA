#pragma once

#include <QDockWidget>

namespace SymbolEditor
{

    class Settings;

    class DockWidget : public QDockWidget
    {
    public:
        DockWidget(QWidget *parent = 0, Qt::WindowFlags flags = 0);

    public slots:
        virtual void applySettings(const Settings &settings);

    };

}
