#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class QTabWidget;
class QSettings;
class AbstractEditor;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void readSettings();
    void writeSettings();

private:
    Ui::MainWindow *ui;
    QTabWidget *m_editorTabWidget;
    void addGraphicsEditor();
    void addPcbEditor();
    void addLogViewer();
    bool maybeSave();

private slots:
    void activateEditor(int tabIndex);

private:
    AbstractEditor *m_activeEditor;

    // QWidget interface
protected:
    void focusInEvent(QFocusEvent *event);
    void focusOutEvent(QFocusEvent *event);
    bool focusNextPrevChild(bool next);
    virtual void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
