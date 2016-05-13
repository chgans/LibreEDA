#ifndef DOCKWIDGET_H
#define DOCKWIDGET_H

#include <QDockWidget>

class SchSettings;

class DockWidget : public QDockWidget
{
public:
    DockWidget();

public slots:
    void applySettings(const SchSettings &settings);

};

#endif // DOCKWIDGET_H
