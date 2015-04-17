#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class QTabWidget;

class AbstractEditor;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTabWidget *m_editorTabWidget;
    void addGraphicsEditor();
    void addPcbEditor();
    void addLogViewer();

private slots:
    void activateEditor(int tabIndex);

private:
    AbstractEditor *m_activeEditor;

    // QWidget interface
protected:
    void focusInEvent(QFocusEvent *event);
    void focusOutEvent(QFocusEvent *event);
    bool focusNextPrevChild(bool next);
};

#endif // MAINWINDOW_H
