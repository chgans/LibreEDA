#pragma once

#include "core/editormanager/ieditor.h"
#include "document.h"

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

    class Editor : public IEditor
    {
        Q_OBJECT
    public:
        explicit Editor(QObject *parent = nullptr);
        ~Editor();

        View *view() const;
        Scene *scene() const;

    private:

    signals:

    public slots:
        void applySettings(const Settings &settings);

    private:
        Document *m_document = nullptr;
        SnapManager *m_snapManager = nullptr;
        QUndoStack *m_undoStack;

        void addInteractiveTools();
        void addInteractiveTool(InteractiveTool *tool);
        QActionGroup *m_interactiveActionGroup = nullptr;
        QToolBar *m_interactiveToolBar = nullptr;
        SelectTool *m_selectTool;
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

    private slots:
        void addDocumentItem(quint64 id, const Document::Item *item);
        void updateDocumentItem(quint64 id, const Document::Item *item);
        void removeDocumentItem(quint64 id);

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
