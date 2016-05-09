#include "scheditorfactory.h"
#include "scheditorconstants.h"
#include "scheditor.h"

SchEditorFactory::SchEditorFactory(QObject *parent) :
    IEditorFactory(parent)
{
    setId(SCHEDITOR_ID);
    setFileExtension(SCHEDITOR_EXT);
    setDisplayName("SCH editor");
}

void SchEditorFactory::loadSettings()
{
    for (auto editor: m_editors)
    {
        editor->loadSettings();
    }
}

IEditor *SchEditorFactory::createEditor()
{
    auto editor = new SchEditor();
    editor->loadSettings();
    m_editors.append(editor);
    connect(editor, &QObject::destroyed,
            this, [this, editor](QObject *) {
        m_editors.removeOne(editor);
    });
    return editor;
}
