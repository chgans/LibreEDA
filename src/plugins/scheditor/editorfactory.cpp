#include "editorfactory.h"
#include "constants.h"
#include "editor.h"
#include "settings.h"

#include "core/core.h"

using namespace SymbolEditor;

EditorFactory::EditorFactory(QObject *parent) :
    IEditorFactory(parent)
{
    setId(SCHEDITOR_ID);
    setFileExtension(SCHEDITOR_EXT);
    setDisplayName("SCH editor");
}

void EditorFactory::applySettings()
{
    Settings settings;
    settings.load(Core::settings());
    for (auto editor : m_editors)
    {
        editor->applySettings(settings);
    }
}

IEditor *EditorFactory::createEditor()
{
    auto editor = new Editor();
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
