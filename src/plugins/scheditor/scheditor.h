#ifndef GRAPHICSEDITOR_H
#define GRAPHICSEDITOR_H

#include "core/editormanager/ieditor.h"
#include "scheditordocument.h"

class QMainWindow;
class QActionGroup;
class QToolBar;
class QComboBox;
class QUndoStack;

namespace SymbolEditor
{
    class View;
    class Scene;
    class InteractiveTool;
    class PlacementTool;
    class SelectTool;
    class TaskDockWidget;
    class UndoDockWidget;
    class SnapManager;
    class Settings;

    class SchEditor : public IEditor
    {
        Q_OBJECT
    public:
        explicit SchEditor(QObject *parent = nullptr);
        ~SchEditor();

        View *view() const;
        Scene *scene() const;

    private:

    signals:

    public slots:
        void applySettings(const Settings &settings);

    private:
        SchEditorDocument *m_document = nullptr;
        SnapManager *m_snapManager = nullptr;
        QUndoStack *m_undoStack;

        void addInteractiveTools();
        QActionGroup *m_interactiveActionGroup = nullptr;
        QToolBar *m_interactiveToolBar = nullptr;
        SelectTool *m_selectTool;
        QList<PlacementTool *> m_placementTools;
        QList<InteractiveTool *> m_interactiveTools;
        void setInteractiveTool(InteractiveTool *tool);
        InteractiveTool * m_interactiveTool = nullptr;

        void addSnapTools();
        QToolBar *m_snapToolBar = nullptr;

        void addPathPointTools();
        QToolBar *m_pathPointToolBar = nullptr;

        void addArrangeTools();
        QToolBar *m_arrangeToolBar = nullptr;

        void addDockWidgets();
        TaskDockWidget *m_taskDockWidget = nullptr;
        UndoDockWidget *m_undoDockWidget;

        void addView();
        View *m_view = nullptr;

        void addScene();
        Scene *m_scene = nullptr;

        // IEditor interface
    public:
        bool open(QString *errorString, const QString &fileName);
        IDocument *document() const;
        QIcon icon() const;
        QString displayName() const;
        void activate(QMainWindow *mainWindow);
        void desactivate(QMainWindow *mainWindow);
    };
}
#endif // GRAPHICSEDITOR_H
