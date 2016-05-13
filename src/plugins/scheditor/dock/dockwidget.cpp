#include "dockwidget.h"

DockWidget::DockWidget(QWidget *parent, Qt::WindowFlags flags):
    QDockWidget(parent, flags)
{

}

void DockWidget::applySettings(const SchEditorSettings &settings)
{
    Q_UNUSED(settings);
}
