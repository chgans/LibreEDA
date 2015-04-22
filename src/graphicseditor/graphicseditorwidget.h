#ifndef GRAPHICSEDITORWIDGET_H
#define GRAPHICSEDITORWIDGET_H

#include "core/abstracteditor.h"

class QMainWindow;
class QActionGroup;
class QToolBar;

class GraphicsView;
class GraphicsScene;
class AbstractGraphicsInteractiveTool;
class TaskDockWidget;

class GraphicsEditorWidget: public AbstractEditor
{
    Q_OBJECT

public:
    GraphicsEditorWidget(QWidget *parent = 0);
    ~GraphicsEditorWidget();

    void activate(QMainWindow *win);
    void desactivate(QMainWindow *win);

    void readSettings(QSettings &settings);
    void writeSettings(QSettings &settings);

    QString type() const;


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

#endif // GRAPHICSEDITORWIDGET_H
