#include "editorview.h"
#include "ieditor.h"
#include "idocument.h"

#include <QTabWidget>
#include <QVBoxLayout>

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
    IDocument *doc = editor->document();
    int index = m_tabWidget->addTab(widget, doc->displayName());
    m_widgetEditorMap.insert(widget, editor);
    m_editorTabIndexMap.insert(editor, index);
}

void EditorView::removeEditor(IEditor *editor)
{
    QWidget *widget = editor->widget();
    m_tabWidget->removeTab(m_editorTabIndexMap.value(editor));
    m_widgetEditorMap.remove(widget);
    m_editorTabIndexMap.remove(editor);
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

IEditor *EditorView::editorForDocument(const IDocument *document) const
{
    foreach (IEditor *editor, editors()) {
        if (editor->document() == document)
            return editor;
    }
    return nullptr;
}

void EditorView::onTabCloseRequested(int index)
{
    QWidget *widget = m_tabWidget->widget(index);
    m_tabWidget->removeTab(index);
    IEditor *editor = m_widgetEditorMap.value(widget);
    removeEditor(editor);
    // TODO: call EditorManager closeEditor() which will manage file saving and IEditor deletion
}

void EditorView::onCurrentTabChanged(int /*index*/)
{
    QWidget *widget = m_tabWidget->currentWidget();
    IEditor *editor = m_widgetEditorMap.value(widget);
    emit currentEditorChanged(editor);
}

