#include "propertyeditordockwidget.h"
#include "propertyeditor/itempropertyeditor.h"

PropertyEditorDockWidget::PropertyEditorDockWidget(QWidget *parent, Qt::WindowFlags flags):
    DockWidget(parent, flags)
{
    setWindowTitle("Property editor");
    m_editor = new ItemPropertyEditor;
    setWidget(m_editor);
}

void PropertyEditorDockWidget::setItem(SchItem *item)
{
    m_editor->setItem(item);
}

void PropertyEditorDockWidget::applySettings(const SchEditorSettings &settings)
{
    Q_UNUSED(settings);
}
