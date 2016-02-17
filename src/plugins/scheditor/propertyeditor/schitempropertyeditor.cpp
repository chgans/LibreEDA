#include "schitempropertyeditor.h"

SchItemPropertyEditor::SchItemPropertyEditor(QWidget *parent) : QWidget(parent)
{

}

SchItemPropertyEditor::~SchItemPropertyEditor()
{

}

SchItem *SchItemPropertyEditor::object() const
{
    return m_object;
}

void SchItemPropertyEditor::setObject(SchItem *object)
{
    m_object = object;
}

void SchItemPropertyEditor::setPropertyValue(const QString &name, const QVariant &value, bool changed)
{
    Q_UNUSED(name)
    Q_UNUSED(value)
    Q_UNUSED(changed)
}
