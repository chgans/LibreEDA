#pragma once

#include "dock/dockwidget.h"

namespace SymbolEditor
{

    class TaskDockWidget : public DockWidget
    {
        Q_OBJECT

    public:
        explicit TaskDockWidget(QWidget *parent = 0, Qt::WindowFlags flags = 0);

    signals:

    public slots:
        void setTaskWidgets(const QList<QWidget *> widgets);

    private:
        QList<QWidget *> m_widgets;

        // DockWidget interface
    public slots:
        void applySettings(const Settings &settings);
    };

}
