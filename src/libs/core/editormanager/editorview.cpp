#include "editorview.h"
#include "ieditor.h"

#include <QTabWidget>
#include <QVBoxLayout>

/*!
  \class EditorView
  \preliminary
  \inmodule LibreEDA
  \ingroup LeEditorManager
  \brief The EditorView class ...
*/

EditorView::EditorView(QWidget *parent) : QWidget(parent)
{
    setLayout(new QVBoxLayout);
    m_tabWidget = new QTabWidget();
    layout()->addWidget(m_tabWidget);
    m_tabWidget->setMovable(true);
    m_tabWidget->setDocumentMode(true);
    m_tabWidget->setTabsClosable(true);
    connect(m_tabWidget, &QTabWidget::tabCloseRequested,
            this, &EditorView::onTabCloseRequested);
    connect(m_tabWidget, &QTabWidget::currentChanged,
            this, &EditorView::onCurrentTabChanged);
}

EditorView::~EditorView()
{

}

int EditorView::editorCount() const
{
    return m_tabWidget->count();
}

void EditorView::addEditor(IEditor *editor)
{
    QWidget *widget = editor->widget();
    int index = m_tabWidget->addTab(widget, editor->icon(), editor->displayName());
    m_editorTabIndexMap.insert(editor, index);
    m_widgetEditorMap.insert(editor->widget(), editor);
}

void EditorView::removeEditor(IEditor *editor)
{
    m_tabWidget->removeTab(m_editorTabIndexMap.value(editor));
    m_editorTabIndexMap.remove(editor);
    m_widgetEditorMap.remove(editor->widget());
}

IEditor *EditorView::currentEditor() const
{
    QWidget *widget = m_tabWidget->currentWidget();
    return m_widgetEditorMap.value(widget);
}

void EditorView::setCurrentEditor(IEditor *editor)
{
    QWidget *widget = editor->widget();
    m_tabWidget->setCurrentWidget(widget);
}

bool EditorView::hasEditor(IEditor *editor) const
{
    return editors().contains(editor);
}

QList<IEditor *> EditorView::editors() const
{
    return m_editorTabIndexMap.keys();
}

void EditorView::onTabCloseRequested(int index)
{
    QWidget *widget = m_tabWidget->widget(index);
    IEditor *editor = m_widgetEditorMap.value(widget);
    emit editorCloseRequested(editor);
}

void EditorView::onCurrentTabChanged(int /*index*/)
{
    QWidget *widget = m_tabWidget->currentWidget();
    IEditor *editor = m_widgetEditorMap.value(widget);
    emit currentEditorChanged(editor);
}

