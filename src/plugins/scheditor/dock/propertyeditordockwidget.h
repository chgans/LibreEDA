#pragma once

#include "scheditor/dock/dockwidget.h"

#include <QDockWidget>

namespace SymbolEditor
{

    class ItemPropertyEditor;
    class Item;

    class PropertyEditorDockWidget : public DockWidget
    {
        Q_OBJECT

    public:
        PropertyEditorDockWidget(QWidget *parent = 0, Qt::WindowFlags flags = 0);

    signals:

    public slots:
        void setItem(Item *item);

    private:
        ItemPropertyEditor *m_editor;

        // DockWidget interface
    public slots:
        void applySettings(const Settings &settings);
    };

}
