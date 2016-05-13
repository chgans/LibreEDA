#ifndef GRAPHICSEDITOR_H
#define GRAPHICSEDITOR_H

#include "core/editormanager/ieditor.h"
#include "scheditordocument.h"

class QMainWindow;
class QActionGroup;
class QToolBar;
class QComboBox;
class QUndoStack;

class SchView;
class SchScene;
class InteractiveTool;
class TaskDockWidget;
class PropertyEditorDockWidget;
class UndoDockWidget;
class SnapManager;
class SchEditorSettings;

class SchEditor : public IEditor
{
    Q_OBJECT
public:
    explicit SchEditor(QObject *parent = nullptr);
    ~SchEditor();

    SchView *view() const;
    SchScene *scene() const;

private:

signals:

public slots:
    void applySettings(const SchEditorSettings &settings);

private:
    SchEditorDocument *m_document = nullptr;
    SnapManager *m_snapManager = nullptr;
    QUndoStack *m_undoStack;

    void addInteractiveTools();
    void addInteractiveTool(InteractiveTool *tool);
    QActionGroup *m_interactiveToolsActionGroup = nullptr;
    QList<InteractiveTool *> m_interactiveTools;
    QToolBar *m_interactiveToolsToolBar = nullptr;

    void addSnapTools();
    QToolBar *m_snapToolBar = nullptr;

    void addPathPointTools();
    QToolBar *m_pathPointToolBar = nullptr;

    void addArrangeTools();
    QToolBar *m_arrangeToolBar = nullptr;

    void addDockWidgets();
    TaskDockWidget *m_taskDockWidget = nullptr;
    PropertyEditorDockWidget *m_propertyEditorDockWidget = nullptr;
    UndoDockWidget *m_undoDockWidget;

    void addView();
    SchView *m_view = nullptr;

    void addScene();
    SchScene *m_scene = nullptr;

    // IEditor interface
public:
    bool open(QString *errorString, const QString &fileName);
    IDocument *document() const;
    QIcon icon() const;
    QString displayName() const;
    void activate(QMainWindow *mainWindow);
    void desactivate(QMainWindow *mainWindow);
};

#endif // GRAPHICSEDITOR_H
