#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class EditorManager;
class EditorView;
class QSettings;

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

private:
    EditorManager *m_editorManager;
    EditorView *m_editorView;

    // QWidget interface
protected:
    virtual void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
