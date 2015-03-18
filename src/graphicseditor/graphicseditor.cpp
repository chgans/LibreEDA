#include "graphicseditor.h"
#include "graphicsview.h"
#include "graphicsscene.h"
#include "graphicsgrid.h"

#include "tool/graphicsbeziertool.h"
#include "tool/graphicsselecttool.h"
#include "tool/graphicslinetool.h"
#include "tool/graphicsrecttool.h"

#include "grid/graphicscartesiangrid.h"

#include <QMainWindow>
#include <QVBoxLayout>
#include <QAction>
#include <QActionGroup>
#include <QToolBar>

GraphicsEditor::GraphicsEditor(QWidget *parent):
    QWidget(parent),
    m_interactiveToolsActionGroup(new QActionGroup(this)),
    m_interactiveToolsToolBar(new QToolBar(this)),
    m_snapToolBar(new QToolBar(this))
{
    setLayout(new QVBoxLayout);
    m_scene = new GraphicsScene(this);
    m_scene->setSceneRect(0, 0, 297, 210);
    GraphicsCartesianGrid *grid = new GraphicsCartesianGrid();
    grid->setOrigin(QPointF(0, 0));
    grid->setSize(QSize(m_scene->sceneRect().width(),
                        m_scene->sceneRect().height()));
    grid->setQuadrantEnabled(GraphicsCartesianGrid::TopRightQuadrant, false);
    grid->setQuadrantEnabled(GraphicsCartesianGrid::BottomLeftQuadrant, false);
    m_scene->setGrid(grid);
    m_view = new GraphicsView();
    m_view->setScene(m_scene);
    //m_view->fitInView(m_scene->sceneRect());
    layout()->addWidget(m_view);

    addInteractiveTool(new GraphicsSelectTool(this));
    addInteractiveTool(new GraphicsLineTool(this));
    addInteractiveTool(new GraphicsRectTool(this));
    addInteractiveTool(new GraphicsBezierTool(this));

    QAction *action;
    action = new QAction(QIcon(":/icons/graphicssnaplock.svg"),
                         "Snap On/Off", nullptr);
    action->setCheckable(true);
    action->setChecked(true);
    m_snapToolBar->addAction(action);
    action = new QAction(QIcon(":/icons/graphicssnapgrid.svg"),
                         "Snap on grid", nullptr);
    action->setCheckable(true);
    action->setChecked(true);
    m_snapToolBar->addAction(action);
    connect(action, &QAction::triggered,
            m_view, &GraphicsView::enableSnapToGrid);
    action = new QAction(QIcon(":/icons/graphicssnapobject.svg"),
                         "Snap on object hot spots", nullptr);
    action->setCheckable(true);
    action->setChecked(true);
    m_snapToolBar->addAction(action);
    action = new QAction(QIcon::fromTheme("preferences-system"),
                         "Configure grid and snapping", nullptr);
    m_snapToolBar->addAction(action);

    Q_INIT_RESOURCE(graphicseditor);
}

GraphicsEditor::~GraphicsEditor()
{

}

void GraphicsEditor::activate(QMainWindow *win)
{
    win->addToolBar(m_interactiveToolsToolBar);
    win->addToolBar(m_snapToolBar);
}

void GraphicsEditor::desactivate(QMainWindow *win)
{
    win->removeToolBar(m_snapToolBar);
    win->removeToolBar(m_interactiveToolsToolBar);
}

void GraphicsEditor::addInteractiveTool(GraphicsTool *tool)
{
    bool firstTool = m_interactiveTools.count() == 0;
    bool firstAction = m_interactiveToolsActionGroup->actions().count() == 0;
    QAction *action = tool->action();
    action->setCheckable(true);
    action->setData(QVariant::fromValue<GraphicsTool *>(tool));
    m_interactiveToolsActionGroup->addAction(action);
    m_interactiveToolsToolBar->addAction(action);
    if (firstAction) {
        action->setChecked(true);
        firstAction = false;
    }
    else
        action->setChecked(false);
    connect(m_interactiveToolsActionGroup, &QActionGroup::triggered,
            this, [this](QAction *action) {
        GraphicsTool *tool = action->data().value<GraphicsTool*>();
        m_view->setTool(tool);
    });
    connect(tool, &GraphicsTool::finished,
            this, [this]() {
        // Todo: activate select tool
    });
    m_interactiveTools.append(tool);
    if (firstTool)
        m_view->setTool(tool);
}
