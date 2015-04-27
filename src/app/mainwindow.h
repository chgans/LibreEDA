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
    void openFileRequested(bool);
    void populateRecentFilesMenu();
    void onEditorOpened(IEditor *editor);
    void onEditorCloseRequested(IEditor *editor);

private:
    EditorView *m_editorView;
    NavigationDockWidget *m_navigationDockWidget;
    QMenu *m_recentFilesMenu;

    // QWidget interface
protected:
    virtual void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
