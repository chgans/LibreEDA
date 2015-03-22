#include "graphicseditor.h"
#include "graphicsview.h"
#include "graphicsscene.h"
#include "graphicsgrid.h"

#include "tool/graphicsbeziertool.h"
#include "tool/graphicsselecttool.h"
#include "tool/graphicslinetool.h"
#include "tool/graphicsrecttool.h"
#include "tool/graphicscircletool.h"

#include "grid/graphicscartesiangrid.h"

#include <QMainWindow>
#include <QVBoxLayout>
#include <QAction>
#include <QActionGroup>
#include <QToolBar>

GraphicsEditor::GraphicsEditor(QWidget *parent):
    QWidget(parent)
{
    setLayout(new QVBoxLayout);
    m_scene = new GraphicsScene(this);
    m_scene->setSceneRect(0, 0, 297, 210);
    GraphicsCartesianGrid *grid = new GraphicsCartesianGrid();
    grid->setOrigin(QPointF(0, 0));
    grid->setSize(QSize(m_scene->sceneRect().width(),
                        m_scene->sceneRect().height()));
    m_scene->setGrid(grid);
    m_view = new GraphicsView();
    m_view->setScene(m_scene);
    m_view->fitInView(m_scene->sceneRect(), Qt::KeepAspectRatio);
    m_view->ensureVisible(m_scene->sceneRect());
    layout()->addWidget(m_view);

    addInteractiveTools();
    addSnapTools();
    addPathPointTools();

    Q_INIT_RESOURCE(graphicseditor);
}

GraphicsEditor::~GraphicsEditor()
{

}

void GraphicsEditor::activate(QMainWindow *win)
{
    win->addToolBar(m_interactiveToolsToolBar);
    win->addToolBar(m_snapToolBar);
    win->addToolBar(m_pathPointToolBar);
}

void GraphicsEditor::desactivate(QMainWindow *win)
{
    win->removeToolBar(m_pathPointToolBar);
    win->removeToolBar(m_snapToolBar);
    win->removeToolBar(m_interactiveToolsToolBar);
}

void GraphicsEditor::addInteractiveTools()
{
    m_interactiveToolsActionGroup = new QActionGroup(this);
    m_interactiveToolsToolBar = new QToolBar();

    addInteractiveTool(new GraphicsSelectTool(this));
    addInteractiveTool(new GraphicsLineTool(this));
    addInteractiveTool(new GraphicsRectTool(this));
    addInteractiveTool(new GraphicsCircleTool(this));
    addInteractiveTool(new GraphicsBezierTool(this));

    QAction *action;
    action = new QAction(QIcon(":/icons/graphicspolygontool.svg"),
                         "Add a polygon", nullptr);
    m_interactiveToolsToolBar->addAction(action); // regular polygon, TODO: add arbitrary polygone and "advanced" shape
    action = new QAction(QIcon(":/icons/graphicspolylinetool.svg"),
                         "Add a polyline", nullptr);
    m_interactiveToolsToolBar->addAction(action);
    action = new QAction(QIcon(":/icons/graphicsellipsetool.svg"),
                         "Add an ellipse", nullptr);
    m_interactiveToolsToolBar->addAction(action);
    action = new QAction(QIcon(":/icons/graphicsarctool.svg"),
                         "Add an arc", nullptr);
    m_interactiveToolsToolBar->addAction(action);
    action = new QAction(QIcon(":/icons/graphicsbeziercurvetool.svg"),
                         "Add a a bezier curve", nullptr);
    m_interactiveToolsToolBar->addAction(action);
    action = new QAction(QIcon(":/icons/graphicsbeziersplinetool.svg"), // TODO: rename to basisspline
                         "Add a basis spline (B-spline)", nullptr);
    m_interactiveToolsToolBar->addAction(action);

    // TODO:
    //  - polyline => wire
    //  - wire: add "make junction" and "make bridge"
    //  - wire mode: 90 deg, 45 deg, bezier
    //  - BSpline vs bezier curve
    //  - create new data class alongside QPoint, QLine, QRect, ...: Ellipse, circle, arc
    //  - with easy convert to/from QPolygon, QRect, QPainterPath, etc...
}

void GraphicsEditor::addInteractiveTool(AbstractGraphicsInteractiveTool *tool)
{

    bool firstTool = m_interactiveTools.count() == 0;
    bool firstAction = m_interactiveToolsActionGroup->actions().count() == 0;
    QAction *action = tool->action();
    action->setCheckable(true);
    action->setData(QVariant::fromValue<AbstractGraphicsInteractiveTool *>(tool));
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
        AbstractGraphicsInteractiveTool *tool = action->data().value<AbstractGraphicsInteractiveTool*>();
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

void GraphicsEditor::addSnapTools()
{
    QAction *action;

    m_snapToolBar = new QToolBar();

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
    m_view->enableSnapToGrid(action->isChecked());

    action = new QAction(QIcon(":/icons/graphicssnapobject.svg"),
                         "Snap on object hot spots", nullptr);
    action->setCheckable(true);
    action->setChecked(true);
    m_snapToolBar->addAction(action);

    action = new QAction(QIcon::fromTheme("preferences-system"),
                         "Configure grid and snapping", nullptr);
    m_snapToolBar->addAction(action);

}

void GraphicsEditor::addPathPointTools()
{
    QAction *action;

    m_pathPointToolBar = new QToolBar();

    action = new QAction(QIcon(":/icons/graphicspathpointadd.svg"),
                         "Add a point to an existing path", nullptr);
    m_pathPointToolBar->addAction(action);

    action = new QAction(QIcon(":/icons/graphicspathpointdel.svg"),
                         "Delete a point from an existing path", nullptr);
    m_pathPointToolBar->addAction(action);

    action = new QAction(QIcon(":/icons/graphicspathpointcorner.svg"),
                         "Make selected path points corner", nullptr);
    m_pathPointToolBar->addAction(action);

    action = new QAction(QIcon(":/icons/graphicspathpointsymetrical.svg"),
                         "Make selected path points symetrical", nullptr);
    m_pathPointToolBar->addAction(action);

    action = new QAction(QIcon(":/icons/graphicspathpointsmooth.svg"),
                         "Make selected path points smooth", nullptr);
    m_pathPointToolBar->addAction(action);

    action = new QAction(QIcon(":/icons/graphicspathpointsmooth.svg"),
                         "Make selected path points auto-smooth", nullptr);
    m_pathPointToolBar->addAction(action);
}

// They all work on one or more items
void GraphicsEditor::addArrangeTools()
{
    // group/ungroup
    // send to back. front, raise, lower
    // align and distribute
    //
    QAction *action = new QAction(QIcon::fromTheme(""),
                         "Make selected path points auto-smooth", nullptr);
    m_pathPointToolBar->addAction(action);

}
