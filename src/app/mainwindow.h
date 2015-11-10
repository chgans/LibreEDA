#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class EditorManager;
class EditorView;
class IEditor;
class NavigationDockWidget;

class QSettings;
class QAction;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void readSettings();
    void writeSettings();

private:

private slots:
    void onNewFileRequested();
    void onOpenFileRequested();
    void onRecentFilesRequested();
    void onSaveCurrentFileRequested();
    void onSaveCurrentFileAsRequested();
    void onSaveAllFilesRequested();
    void onRevertCurrentFileRequested();
    void onReloadCurrentFileRequested();
    void onCloseCurrentFileRequested();
    void onCloseAllExceptCurrentFileRequested();
    void onCloseAllFilesRequested();
    void onPrintCurrentFileRequested();
    void onApplicationExitRequested();

    void onEditorOpened(IEditor *editor);
    void onCurrentEditorChanged(IEditor *editor);
    void onEditorCloseRequested(IEditor *editor);
    void onEditorAboutToClose(IEditor *editor);

private:
    EditorView *m_editorView;
    IEditor *m_currentEditor = nullptr;
    NavigationDockWidget *m_navigationDockWidget;

    QMenu *m_fileMenu;
    QAction *m_saveAction;
    QAction *m_saveAsAction;
    QAction *m_saveAllAction;
    QAction *m_closeAction;
    QAction *m_closeAllAction;
    QAction *m_closeAllExceptAction;

    QMenu *m_recentFilesMenu;
    QMenu *m_editMenu;
    QMenu *m_windowMenu;
    QMenu *m_helpMenu;

    void updateEditorActions();

    // QWidget interface
protected:
    virtual void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
