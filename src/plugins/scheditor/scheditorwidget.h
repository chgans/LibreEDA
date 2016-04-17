#ifndef GRAPHICSEDITORWIDGET_H
#define GRAPHICSEDITORWIDGET_H

#include "core/abstracteditor.h"

class QMainWindow;
class QActionGroup;
class QToolBar;
class QComboBox;

class SchView;
class SchScene;
class AbstractGraphicsInteractiveTool;
class TaskDockWidget;
class PropertyEditorDockWidget;
class SnapManager;

class SchEditorWidget: public AbstractEditor
{
    Q_OBJECT

public:
    SchEditorWidget(QWidget *parent = 0);
    ~SchEditorWidget();

    void activate(QMainWindow *win);
    void desactivate(QMainWindow *win);

    void readSettings(QSettings &settings);
    void writeSettings(QSettings &settings);

    QString type() const;

    SchView *view() const;
    SchScene *scene() const;

private:
    QMainWindow *m_mainWindow = nullptr;
    SchView *m_view = nullptr;
    SchScene *m_scene = nullptr;
    TaskDockWidget *m_taskDockWidget = nullptr;
    PropertyEditorDockWidget *m_propertyEditorDockWidget = nullptr;

    void addInteractiveTools();
    void addInteractiveTool(AbstractGraphicsInteractiveTool *tool);
    QActionGroup *m_interactiveToolsActionGroup = nullptr;
    QList<AbstractGraphicsInteractiveTool *> m_interactiveTools;
    QToolBar *m_interactiveToolsToolBar = nullptr;

    SnapManager *m_snapManager = nullptr;
    void addSnapTools();
    QToolBar *m_snapToolBar = nullptr;

    void addPathPointTools();
    QToolBar *m_pathPointToolBar = nullptr;

    void addArrangeTools();
    QToolBar *m_arrangeToolBar = nullptr;

    // QWidget interface
protected:
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);
};

#endif // GRAPHICSEDITORWIDGET_H
