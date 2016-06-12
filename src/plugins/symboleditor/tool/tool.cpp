#include "tool/tool.h"

#include "view/scene.h"
#include "view/view.h"
#include "document.h"
#include "editor.h"
#include <QDialog>

namespace SymbolEditor
{

    Tool::Tool(Editor *editor) :
        QObject(editor),
        m_editor(editor)
    {

    }

    Tool::~Tool()
    {

    }

    View *Tool::view()
    {
        return m_editor->view();
    }

    Scene *Tool::scene()
    {
        return m_editor->scene();
    }

    const Document *Tool::document()
    {
        return static_cast<const Document*>(m_editor->document());
    }

    void Tool::setToolGroup(const QString &group)
    {
        m_toolGroup = group;
    }

    QString Tool::toolGroup() const
    {
        return m_toolGroup;
    }

    void Tool::setAction(QAction *action)
    {
        m_action = action;
    }

    QAction *Tool::action() const
    {
        return m_action;
    }

    void Tool::cancel()
    {

    }

    void Tool::applySettings(const Settings &settings)
    {
        Q_UNUSED(settings);
    }

}
