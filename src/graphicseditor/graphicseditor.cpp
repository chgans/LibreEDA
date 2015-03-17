#include "graphicseditor.h"
#include "graphicsview.h"
#include "graphicsscene.h"
#include "tool/graphicsbeziertool.h"
#include "tool/graphicsselecttool.h"
#include "tool/graphicslinetool.h"
#include "tool/graphicsrecttool.h"

#include <QMainWindow>
#include <QVBoxLayout>
#include <QAction>
#include <QActionGroup>
#include <QToolBar>

GraphicsEditor::GraphicsEditor(QWidget *parent):
    QWidget(parent),
    m_interactiveToolsActionGroup(new QActionGroup(this)),
    m_interactiveToolsToolBar(new QToolBar(this))
{
    setLayout(new QVBoxLayout);
    m_scene = new GraphicsScene(this);
    m_scene->setSceneRect(0, 0, 297, 210);
    m_view = new GraphicsView();
    m_view->setScene(m_scene);
    m_view->scale(4, 4);
    layout()->addWidget(m_view);

    addTool(new GraphicsSelectTool(this));
    addTool(new GraphicsLineTool(this));
    addTool(new GraphicsRectTool(this));
    addTool(new GraphicsBezierTool(this));

    Q_INIT_RESOURCE(graphicseditor);
}

GraphicsEditor::~GraphicsEditor()
{

}

void GraphicsEditor::activate(QMainWindow *win)
{
    win->addToolBar(m_interactiveToolsToolBar);
}

void GraphicsEditor::desactivate(QMainWindow *win)
{
    win->removeToolBar(m_interactiveToolsToolBar);
}

void GraphicsEditor::addTool(GraphicsTool *tool)
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
