#include "dockwidget.h"

namespace SymbolEditor
{

    DockWidget::DockWidget(QWidget *parent, Qt::WindowFlags flags):
        QDockWidget(parent, flags)
    {

    }

    void DockWidget::applySettings(const Settings &settings)
    {
        Q_UNUSED(settings);
    }

}
