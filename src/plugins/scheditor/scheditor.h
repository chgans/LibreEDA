#ifndef GRAPHICSEDITOR_H
#define GRAPHICSEDITOR_H

#include "core/editormanager/ieditor.h"

class QMainWindow;
class QActionGroup;
class QToolBar;
class QComboBox;

class SchEditorDocument;
class SchView;
class SchScene;
class AbstractGraphicsInteractiveTool;
class TaskDockWidget;
class PropertyEditorDockWidget;
class SnapManager;

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
    void loadSettings();

private:
    SchEditorDocument *m_document = nullptr;
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
