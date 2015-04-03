#ifndef GRAPHICSEDITOR_H
#define GRAPHICSEDITOR_H

#include <QWidget>

class QMainWindow;
class QActionGroup;
class QToolBar;

class GraphicsView;
class GraphicsScene;
class AbstractGraphicsInteractiveTool;
class TaskDockWidget;

class GraphicsEditor: public QWidget
{
    Q_OBJECT

public:
    GraphicsEditor(QWidget *parent = 0);
    ~GraphicsEditor();

public slots:
    void activate(QMainWindow *win);
    void desactivate(QMainWindow *win);

private:
    QMainWindow *m_mainWindow;
    GraphicsView *m_view;
    GraphicsScene *m_scene;
    TaskDockWidget *m_taskDockWidget;

    void addInteractiveTools();
    void addInteractiveTool(AbstractGraphicsInteractiveTool *tool);
    QActionGroup *m_interactiveToolsActionGroup;
    QList<AbstractGraphicsInteractiveTool *> m_interactiveTools;
    QToolBar *m_interactiveToolsToolBar;

    void addSnapTools();
    QToolBar *m_snapToolBar;

    void addPathPointTools();
    QToolBar *m_pathPointToolBar;

    void addArrangeTools();
    QToolBar *m_arrangeToolBar;
};

#endif // GRAPHICSEDITOR_H
