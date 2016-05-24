#include "scheditorfactory.h"
#include "scheditorconstants.h"
#include "scheditor.h"
#include "settings.h"

#include "core/core.h"

SchEditorFactory::SchEditorFactory(QObject *parent) :
    IEditorFactory(parent)
{
    setId(SCHEDITOR_ID);
    setFileExtension(SCHEDITOR_EXT);
    setDisplayName("SCH editor");
}

void SchEditorFactory::applySettings()
{
    Settings settings;
    settings.load(Core::settings());
    for (auto editor : m_editors)
    {
        editor->applySettings(settings);
    }
}

IEditor *SchEditorFactory::createEditor()
{
    auto editor = new SchEditor();
    m_editors.append(editor);
    connect(editor, &QObject::destroyed,
            this, [this, editor](QObject *)
    {
        m_editors.removeOne(editor);
    });
    Settings settings;
    settings.load(Core::settings());
    editor->applySettings(settings);
    return editor;
}
