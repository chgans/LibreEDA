#ifndef GRAPHICSEDITOR_H
#define GRAPHICSEDITOR_H

#include <QWidget>

class QMainWindow;
class QActionGroup;
class QToolBar;

class GraphicsView;
class GraphicsScene;
class GraphicsTool;

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
    GraphicsView *m_view;
    GraphicsScene *m_scene;

    void addInteractiveTools();
    void addInteractiveTool(GraphicsTool *tool);
    QActionGroup *m_interactiveToolsActionGroup;
    QList<GraphicsTool *> m_interactiveTools;
    QToolBar *m_interactiveToolsToolBar;

    void addSnapTools();
    QToolBar *m_snapToolBar;

    void addPathPointTools();
    QToolBar *m_pathPointToolBar;

    void addArrangeTools();
    QToolBar *m_arrangeToolBar;
};

#endif // GRAPHICSEDITOR_H
