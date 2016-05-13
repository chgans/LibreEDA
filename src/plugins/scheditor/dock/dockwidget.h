#ifndef DOCKWIDGET_H
#define DOCKWIDGET_H

#include <QDockWidget>

class SchEditorSettings;

class DockWidget : public QDockWidget
{
public:
    DockWidget(QWidget *parent = 0, Qt::WindowFlags flags = 0);

public slots:
    virtual void applySettings(const SchEditorSettings &settings);

};

#endif // DOCKWIDGET_H
