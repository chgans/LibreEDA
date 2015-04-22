#ifndef EDITORMANAGER_H
#define EDITORMANAGER_H

#include <QObject>
#include <QMap>
#include <QStringList>

class IEditor;
class IEditorFactory;
class EditorView;
class IDocument;

class QSettings;
class QMainWindow;

class Id;

class EditorManager : public QObject
{
    Q_OBJECT

public:
    static EditorManager *instance();

    static EditorView *createView();
    static IEditor *openEditor(const QString &fileName);
    static QStringList getOpenFileNames();
    static IDocument *currentDocument();
    static IEditor *currentEditor();

    static void activateEditor(IEditor *editor);
    static IEditor *activateEditorForDocument(IDocument *document);

    static bool closeDocument(IDocument *document, bool askAboutModifiedEditors = true);
    static bool closeDocuments(const QList<IDocument *> &documents, bool askAboutModifiedEditors = true);
    static void closeOtherDocuments(IDocument *document);

    static bool saveDocument(IDocument *document);

    static bool closeEditors(const QList<IEditor *> &editors, bool askAboutModifiedEditors = true);
    static void closeEditor(IEditor *editor, bool askAboutModifiedEditors = true);

    static void saveState(QSettings *settings);
    static bool restoreState(QSettings *settings);

    static void registerEditorFactory(IEditorFactory *factory);
    static IEditorFactory *editorFactory(const QString &fileExtension);
    static QStringList supportedFileExtensions();
    static QString supportedFileFilter();

signals:
    void currentEditorChanged(IEditor *editor);
    void currentDocumentStateChanged();
    void editorCreated(IEditor *editor, const QString &fileName);
    void editorOpened(IEditor *editor);
    void editorAboutToClose(IEditor *editor);
    void editorsClosed(QList<IEditor *> editors);

public slots:
    static void saveDocument();
    static void saveDocumentAs();
    static void revertToSaved();
    static bool closeAllEditors(bool askAboutModifiedEditors = true);
    static void slotCloseCurrentDocument();
    static void closeOtherDocuments();

private:
    explicit EditorManager(QObject *parent = nullptr);
    ~EditorManager();
    static IEditor *createEditor(const QString &fileName);
    static void addEditor(IEditor *editor, const QString &fileName);

    static EditorManager *m_instance;
    static QMap<QString, IEditorFactory *> m_factoryMap;
    static QMap<QString, IEditor*> m_filePathEditorMap;
    static QMap<QMainWindow *, EditorView *> m_mainWindowEditorMap;
    static IEditor *m_currentEditor;
};

#endif // EDITORMANAGER_H
