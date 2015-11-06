#ifndef EDITORMANAGER_H
#define EDITORMANAGER_H

#include "core/core_global.h"

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

class CORE_EXPORT EditorManager : public QObject
{
    Q_OBJECT

public:
    static EditorManager *instance();

    static void initialise();

    static IEditor *openEditor(const QString &fileName);
    static bool closeEditor(IEditor *editor);

    static void saveState();
    static bool restoreState();

    static IEditorFactory *editorFactory(const QString &fileExtension);
    static QStringList supportedFileExtensions();
    static QString supportedFileFilter();

signals:
    void editorCreated(IEditor *editor, const QString &fileName);
    void editorOpened(IEditor *editor);
    void editorAboutToClose(IEditor *editor);
    void editorClosed(IEditor * editor);

private:
    explicit EditorManager(QObject *parent = nullptr);
    ~EditorManager();
    static void addEditor(IEditor *editor, const QString &fileName);

    static EditorManager *m_instance;
    static QMap<QString, IEditorFactory *> m_factoryMap;
    static QMap<QString, IEditor *> m_filePathEditorMap; // FIXME: multiple editor on the same file
    static IEditor *m_currentEditor;
};

#endif // EDITORMANAGER_H
