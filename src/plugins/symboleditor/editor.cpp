#include "editor.h"
#include "document.h"
#include "view/view.h"
#include "view/scene.h"
#include "item/item.h"
#include "settings/settings.h"

#include "tool/selecttool.h"
#include "tool/moveitemtool.h"
#include "tool/cloneitemtool.h"
#include "tool/placebeziertool.h"
#include "tool/placepolylinetool.h"
#include "tool/placerectangletool.h"
#include "tool/placecircletool.h"
#include "tool/placeellipsetool.h"
#include "tool/placepolygontool.h"
#include "tool/placearctool.h"

#include "command/command.h"

#include "snap/positionsnapper.h"

#include "dock/taskdockwidget.h"
#include "dock/undodockwidget.h"

#include "core/core.h"

#include <QMainWindow>
#include <QAction>
#include <QToolBar>
#include <QUndoStack>

#include <QFileInfo>
#include <QTimer>

using namespace SymbolEditor;

Editor::Editor(QObject *parent) :
    IEditor(parent),
    m_undoStack(new QUndoStack)
{
    addScene();
    addView();
    setWidget(m_view);
    addDockWidgets();
    addArrangeTools();
    addInteractiveTools();
    addSnapTools();
    addPathPointTools();
}

Editor::~Editor()
{
    qDeleteAll(m_pathPointToolBar->actions());
    qDeleteAll(m_snapToolBar->actions());
    qDeleteAll(m_interactiveToolBar->actions());
    qDeleteAll(m_arrangeToolBar->actions());
    delete m_taskDockWidget;
    delete m_view;
    delete m_scene;
    delete m_document;
}

void Editor::addView()
{
    m_view = new View();
    m_view->setScene(m_scene);
    m_view->fitInView(m_scene->sceneRect(), Qt::KeepAspectRatio);
}

void Editor::addScene()
{
    m_scene = new Scene(this);
    m_scene->setSceneRect(0, 0, 297, 210);
}

void Editor::addDocumentItem(quint64 id, const Document::Item *item)
{
    m_scene->addDocumentItem(id, item);
    m_selectTool->addDocumentItem(id, item);
}

void Editor::updateDocumentItem(quint64 id, const Document::Item *item)
{
    m_scene->updateDocumentItem(id, item);
    m_selectTool->updateDocumentItem(id, item);
}

void Editor::removeDocumentItem(quint64 id)
{
    m_scene->removeDocumentItem(id);
    m_selectTool->removeDocumentItem(id);
}

void Editor::applySettings(const Settings &settings)
{
    m_scene->applySettings(settings);
    m_view->applySettings(settings);
    for (auto tool : m_interactiveTools)
    {
        tool->applySettings(settings);
    }
    // TODO: other tools
    // TODO: snap manager
    m_taskDockWidget->applySettings(settings);
}

bool Editor::open(QString *errorString, const QString &fileName)
{
    m_document = new Document();
    m_document->setFilePath(fileName);
    QFileInfo fileInfo(fileName);
    m_document->setDisplayName(fileInfo.baseName());

    bool result = m_document->load(errorString, m_document->filePath());
    if (!result)
    {
        return false;
    }

    for (quint64 id : m_document->itemIdList())
    {
       addDocumentItem(id, m_document->item(id));
    }

    connect(m_document, &Document::itemAdded,
            this, &Editor::addDocumentItem);
    connect(m_document, &Document::itemChanged,
            this, &Editor::updateDocumentItem);
    connect(m_document, &Document::itemRemoved,
            this, &Editor::removeDocumentItem);

    m_undoDockWidget->setStack(m_undoStack);

    return true;
}

IDocument *Editor::document() const
{
    return m_document;
}

QIcon Editor::icon() const
{
    return QIcon(":/icons/sch.png");
}

QString Editor::displayName() const
{
    return m_document->displayName();
}

void Editor::activate(QMainWindow *win)
{
    win->addToolBar(m_interactiveToolBar);
    m_interactiveToolBar->show();
    win->addToolBar(m_snapToolBar);
    m_snapToolBar->show();
    win->addToolBar(m_arrangeToolBar);
    m_arrangeToolBar->show();
    win->addToolBar(m_pathPointToolBar);
    m_pathPointToolBar->show();
    win->addDockWidget(Qt::RightDockWidgetArea, m_taskDockWidget);
    m_taskDockWidget->show();
    win->addDockWidget(Qt::RightDockWidgetArea, m_undoDockWidget);
    m_undoDockWidget->show();
    win->tabifyDockWidget(m_taskDockWidget, m_undoDockWidget);
}

void Editor::desactivate(QMainWindow *win)
{
    win->removeDockWidget(m_undoDockWidget);
    win->removeDockWidget(m_taskDockWidget);
    win->removeToolBar(m_arrangeToolBar);
    win->removeToolBar(m_pathPointToolBar);
    win->removeToolBar(m_snapToolBar);
    win->removeToolBar(m_interactiveToolBar);
}

void Editor::addInteractiveTools()
{
    // TBD: Select tool could use the Esc shortcut,
    // we need to make sure that the active tool is correctly
    // descativated/cancelled
    // Reponsability is currently spread across Editor, View and Tools

    m_interactiveActionGroup = new QActionGroup(this);
    m_interactiveToolBar = new QToolBar();
    m_selectTool = new SelectTool(this);

    addInteractiveTool(m_selectTool);
    addInteractiveTool(new MoveItemTool(this));
    addInteractiveTool(new CloneItemTool(this));
    addInteractiveTool(new PlaceRectangleTool(this));
    addInteractiveTool(new PlaceCircleTool(this));
    addInteractiveTool(new PlaceEllipseTool(this));
    addInteractiveTool(new PlacePolygonTool(this));

    connect(m_interactiveActionGroup, &QActionGroup::triggered,
            this, [this](QAction * action)
    {
        InteractiveTool *tool = action->data().value<InteractiveTool *>();
        setInteractiveTool(tool);
    });

    m_selectTool->action()->setChecked(true);
    setInteractiveTool(m_selectTool);
}

void Editor::addInteractiveTool(InteractiveTool *tool)
{
    QAction *action = tool->action();
    action->setCheckable(true);
    action->setData(QVariant::fromValue<InteractiveTool *>(tool));
    m_interactiveActionGroup->addAction(action);
    m_interactiveToolBar->addAction(action);
    connect(tool, &Tool::commandRequested,
            this, [this](UndoCommand * command)
    {
        command->setDocument(m_document);
        m_undoStack->push(command);
    });
    connect(tool, &Tool::finished, // TODO: rename to differentiate with taskCompleted, canceled, ...
            m_selectTool->action(), &QAction::trigger);
}

void Editor::setInteractiveTool(InteractiveTool *tool)
{
    if (m_interactiveTool != nullptr)
    {
        m_interactiveTool->disconnect(m_taskDockWidget);
    }

    m_interactiveTool = tool;
    m_view->setTool(m_interactiveTool);

    if (m_interactiveTool != nullptr)
    {
        connect(m_interactiveTool, &InteractiveTool::taskWidgetsChanged,
                m_taskDockWidget, &TaskDockWidget::setTaskWidgets);
        m_taskDockWidget->setTaskWidgets(tool->taskWidgets());
    }
    else
    {
        m_taskDockWidget->setTaskWidgets(QList<QWidget *>());
    }
}

void Editor::addSnapTools()
{
    m_snapManager = m_view->snapManager(); //new SnapManager(m_view);
    m_snapToolBar = new QToolBar();

    for (auto group : m_snapManager->groups())
    {
        m_snapToolBar->addActions(m_snapManager->actions(group));
    }

    return;
}

void Editor::addPathPointTools()
{
    m_pathPointToolBar = new QToolBar();
}

// TODO: group/ungroup. send to back. front, raise, lower. align and distribute.
void Editor::addArrangeTools()
{
    m_arrangeToolBar = new QToolBar();
}

void Editor::addDockWidgets()
{
    // Task dockwidget non-movable and no dock decoration
    m_taskDockWidget = new TaskDockWidget();
    m_taskDockWidget->setAllowedAreas(Qt::RightDockWidgetArea);
    m_taskDockWidget->setFeatures(0);
    m_taskDockWidget->setTitleBarWidget(new QWidget());

    // Undo dockwidget movable with default decoration
    m_undoDockWidget = new UndoDockWidget();
}
