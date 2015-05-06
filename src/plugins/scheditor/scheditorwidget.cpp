#include "scheditorwidget.h"
#include "schview.h"
#include "schscene.h"
#include "graphicsgrid.h"
#include "palette.h"

#include "tool/graphicsbeziertool.h"
#include "tool/graphicsselecttool.h"
#include "tool/graphicslinetool.h"
#include "tool/graphicsrecttool.h"
#include "tool/graphicscircletool.h"
#include "tool/graphicsellipsetool.h"
#include "tool/graphicspolygontool.h"
#include "tool/graphicswiretool.h"
#include "tool/graphicsarctool.h"

#include "grid/graphicscartesiangrid.h"

#include "dock/taskdockwidget.h"

#include <QMainWindow>
#include <QVBoxLayout>
#include <QAction>
#include <QActionGroup>
#include <QToolBar>
#include <QTimer>
#include <QComboBox>

SchEditorWidget::SchEditorWidget(QWidget *parent):
    AbstractEditor(parent)
{
    setLayout(new QVBoxLayout);
    m_scene = new SchScene(this);
    m_scene->setSceneRect(0, 0, 297, 210);
    GraphicsCartesianGrid *grid = new GraphicsCartesianGrid();
    grid->setOrigin(QPointF(0, 0));
    grid->setSize(QSize(m_scene->sceneRect().width(),
                        m_scene->sceneRect().height()));
    m_scene->setGrid(grid);
    grid->setCoarseMultiplier(5);
    m_view = new SchView();
    m_view->setScene(m_scene);
    m_view->fitInView(m_scene->sceneRect(), Qt::KeepAspectRatio);
    m_view->ensureVisible(m_scene->sceneRect());
    m_view->setPaletteMode(Palette::Dark);
    layout()->addWidget(m_view);

    m_taskDockWidget = new TaskDockWidget();
    addInteractiveTools();
    addSnapTools();
    addPathPointTools();

    Q_INIT_RESOURCE(scheditor);
}

SchEditorWidget::~SchEditorWidget()
{

}

void SchEditorWidget::readSettings(QSettings &settings)
{
    Q_UNUSED(settings);
}

void SchEditorWidget::writeSettings(QSettings &settings)
{
    Q_UNUSED(settings);
}

QString SchEditorWidget::type() const
{
    return "graphicseditor";
}

SchView *SchEditorWidget::view() const
{
    return m_view;
}

SchScene *SchEditorWidget::scene() const
{
    return m_scene;
}

void SchEditorWidget::activate(QMainWindow *win)
{
    win->addToolBar(m_interactiveToolsToolBar);
    m_interactiveToolsToolBar->show();
    QToolBar *bar = win->addToolBar("theme");
    m_paletteModeComboBox = new QComboBox;
    m_paletteModeComboBox->addItem("Dark", QVariant::fromValue<Palette::Mode>(Palette::Dark));
    m_paletteModeComboBox->addItem("Light", QVariant::fromValue<Palette::Mode>(Palette::Light));
    m_paletteModeComboBox->setCurrentIndex(0);
    connect(m_paletteModeComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(onPaletteComboBoxIndexChanged(int)));
    bar->addWidget(m_paletteModeComboBox);

#if 0
    win->addToolBar(m_snapToolBar);
    m_snapToolBar->show();
    win->addToolBar(m_pathPointToolBar);
    m_pathPointToolBar->show();
    win->addDockWidget(Qt::LeftDockWidgetArea, m_taskDockWidget);
    m_taskDockWidget->show();
#endif
    m_mainWindow = win;
}

void SchEditorWidget::onPaletteComboBoxIndexChanged(int index)
{
    m_view->setPaletteMode(m_paletteModeComboBox->itemData(index).value<Palette::Mode>());
}

void SchEditorWidget::desactivate(QMainWindow *win)
{
    win->removeDockWidget(m_taskDockWidget);
    win->removeToolBar(m_pathPointToolBar);
    win->removeToolBar(m_snapToolBar);
    win->removeToolBar(m_interactiveToolsToolBar);
}

void SchEditorWidget::addInteractiveTools()
{
    m_interactiveToolsActionGroup = new QActionGroup(this);
    m_interactiveToolsToolBar = new QToolBar();

    addInteractiveTool(new GraphicsSelectTool(this));
    addInteractiveTool(new GraphicsLineTool(this));
    //addInteractiveTool(new GraphicsWireTool(this));
    addInteractiveTool(new GraphicsRectTool(this));
    //addInteractiveTool(new GraphicsPolygonTool(this));
    //addInteractiveTool(new GraphicsCircleTool(this));
    addInteractiveTool(new GraphicsArcTool(this));
    addInteractiveTool(new GraphicsEllipseTool(this));
    //addInteractiveTool(new GraphicsBezierTool(this));

    QAction *action;
    action = new QAction(QIcon(":/icons/graphicsarctool.svg"),
                         "Add an arc", nullptr);
    m_interactiveToolsToolBar->addAction(action);

#if 0
    action = new QAction(QIcon(":/icons/graphicsbeziercurvetool.svg"),
                         "Add a a bezier curve", nullptr);
    m_interactiveToolsToolBar->addAction(action);
    action = new QAction(QIcon(":/icons/graphicsbeziersplinetool.svg"), // TODO: rename to basisspline
                         "Add a basis spline (B-spline)", nullptr);
    m_interactiveToolsToolBar->addAction(action);
#endif

    // TODO:
    //  - polyline => wire
    //  - wire: add "make junction" and "make bridge"
    //  - wire mode: 90 deg, 45 deg, bezier
    //  - BSpline vs bezier curve
    //  - create new data class alongside QPoint, QLine, QRect, ...: Ellipse, circle, arc
    //  - with easy convert to/from QPolygon, QRect, QPainterPath, etc...
    //  - regular polygon, arbitrary polygon and "advanced" shape
}

void SchEditorWidget::addInteractiveTool(AbstractGraphicsInteractiveTool *tool)
{

    bool firstTool = m_interactiveTools.count() == 0;
    bool firstAction = m_interactiveToolsActionGroup->actions().count() == 0;
    QAction *action = tool->action();
    action->setCheckable(true);
    action->setData(QVariant::fromValue<AbstractGraphicsInteractiveTool *>(tool));
    m_interactiveToolsActionGroup->addAction(action);
    m_interactiveToolsToolBar->addAction(action);

    action->setChecked(firstAction);
    if (firstAction) {
        connect(m_interactiveToolsActionGroup, &QActionGroup::triggered,
                this, [this](QAction *action) {
            AbstractGraphicsInteractiveTool *tool = action->data().value<AbstractGraphicsInteractiveTool*>();
            // tool->activate(); Do this here or in view?
            m_view->setTool(tool);
            m_taskDockWidget->setTool(tool);
        });
        firstAction = false;
    }
    else {
        connect(tool, &SchTool::finished,
                this, [this]() {
            m_interactiveTools.first()->action()->trigger();
        });
    }

    m_interactiveTools.append(tool);
    if (firstTool) {
        QTimer *timer = new QTimer();
        timer->setSingleShot(true);
        timer->setInterval(0);
        // We need to activate default tool once editor is fully created *and* visible
        connect(timer, &QTimer::timeout,
                [this, tool, timer]() {
            m_view->setTool(tool);
            m_taskDockWidget->setTool(tool);
            delete timer;
        });
    }
}

void SchEditorWidget::addSnapTools()
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
            m_view, &SchView::enableSnapToGrid);
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

void SchEditorWidget::addPathPointTools()
{
#if 0
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
#endif
}

// They all work on one or more items
void SchEditorWidget::addArrangeTools()
{
    // group/ungroup
    // send to back. front, raise, lower
    // align and distribute
    //
    QAction *action = new QAction(QIcon::fromTheme(""),
                         "Make selected path points auto-smooth", nullptr);
    m_pathPointToolBar->addAction(action);

}
