#include "propertyeditordockwidget.h"
#include "propertyeditor/itempropertyeditor.h"

using namespace SymbolEditor;

PropertyEditorDockWidget::PropertyEditorDockWidget(QWidget *parent, Qt::WindowFlags flags):
    DockWidget(parent, flags)
{
    setWindowTitle("Property editor");
    m_editor = new ItemPropertyEditor;
    setWidget(m_editor);
}

void PropertyEditorDockWidget::setItem(Item *item)
{
    m_editor->setItem(item);
}

void PropertyEditorDockWidget::applySettings(const Settings &settings)
{
    Q_UNUSED(settings);
}
