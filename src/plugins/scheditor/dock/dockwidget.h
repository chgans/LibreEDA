#ifndef DOCKWIDGET_H
#define DOCKWIDGET_H

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

#endif // DOCKWIDGET_H
