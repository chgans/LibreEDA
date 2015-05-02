#include "ieditor.h"

#include <QWidget>

/*!
  \class IEditor
  \preliminary
  \inmodule LibreEDA
  \ingroup LeEditorManager
  \mainclass
  \brief The IEditor class is an interface for providing different editors for
  different file types.

  Classes that implement this interface are for example the editors for
  Scematic files, Layout files and text files.

  Whenever a user wants to edit or create a file, the \c EditorManager scans all
  \c IEditorFactory interfaces for suitable editors. The selected \c IEditorFactory
  is then asked to create an editor, which must implement this interface.

  \sa IEditorFactory, EditorManager, EditorView
*/

/*!
 * Constructs an editor with parent \a parent.
 */
IEditor::IEditor(QObject *parent) : QObject(parent)
{

}

/*!
 * Destroys the editor
 */
IEditor::~IEditor()
{

}

void IEditor::saveState(QSettings *) const
{

}

bool IEditor::restoreState(QSettings *)
{
    return true;
}

/*!
 * The widget implementing the GUI interface for this editor.
 */
QWidget *IEditor::widget() const
{
    return m_widget;
}

/*!
 * Set \a widget as the widget implementing the GUI interface for this editor.
 */
void IEditor::setWidget(QWidget *widget)
{
    m_widget = widget;
}

/*!
 * Returns the icon for this editor
 * \sa IEditor::setIcon()
 */
QIcon IEditor::icon() const
{
    return m_icon;
}

/*!
 * Set the icon for this editor to \a icon
 * \sa IEditor::icon()
 */
void IEditor::setIcon(const QIcon &icon)
{
    m_icon = icon;
}

/*!
 * Returns the user visible description of the document
 * \sa IEditor::setDisplayName()
 */
QString IEditor::displayName() const
{
    return m_displayName;
}

/*!
 * Set the user visible description of the document to \a displayName (usually filename w/o path).
 * \sa IEditor::displayName()
 */
void IEditor::setDisplayName(const QString &displayName)
{
    if (displayName == m_displayName)
        return;
    m_displayName = displayName;
}
