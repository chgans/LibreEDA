#ifndef EDITORVIEW_H
#define EDITORVIEW_H

#include "core/core_global.h"

#include <QWidget>
#include <QMap>

class IEditor;
class IDocument;
class QTabWidget;

class CORE_EXPORT EditorView : public QWidget
{
    Q_OBJECT

public:
    explicit EditorView(QWidget *parent = nullptr);
    virtual ~EditorView();

    int editorCount() const;
    void addEditor(IEditor *editor);
    void removeEditor(IEditor *editor);
    IEditor *currentEditor() const;
    void setCurrentEditor(IEditor *editor);

    bool hasEditor(IEditor *editor) const;

    QList<IEditor *> editors() const;
    IEditor *editorForDocument(const IDocument *document) const;

signals:
    void currentEditorChanged(IEditor *editor);
    void editorCloseRequested(IEditor *editor);

private slots:
    void onTabCloseRequested(int index);
    void onCurrentTabChanged(int index);

private:
    QList<IEditor *> m_editors;
    QMap<QWidget *, IEditor *> m_widgetEditorMap;
    QMap<IEditor *, int> m_editorTabIndexMap;
    QTabWidget *m_tabWidget;
};

#endif // EDITORVIEW_H
