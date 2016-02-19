#include "scheditorwidget.h"
#include "schview.h"
#include "schscene.h"
#include "palette.h"
#include "item/schitem.h"

#include "tool/graphicsbeziertool.h"
#include "tool/graphicsselecttool.h"
#include "tool/graphicslinetool.h"
#include "tool/graphicsrecttool.h"
#include "tool/graphicscircletool.h"
#include "tool/graphicsellipsetool.h"
#include "tool/graphicspolygontool.h"
#include "tool/graphicswiretool.h"
#include "tool/graphicsarctool.h"

#include "snap/positionsnapper.h"

#include "grid/graphicscartesiangrid.h"
#include "grid/graphicsgrid.h"

#include "dock/taskdockwidget.h"
#include "dock/propertyeditordockwidget.h"

#include <QMainWindow>
#include <QVBoxLayout>
#include <QAction>
#include <QActionGroup>
#include <QToolBar>
#include <QTimer>
#include <QComboBox>
#include <QSettings>

#include <QGraphicsSimpleTextItem>
#include <QGraphicsTextItem>

#include "item/graphicstextframeitem.h"


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
    m_propertyEditorDockWidget = new PropertyEditorDockWidget();
#if 1
    connect(m_scene, &SchScene::selectionChanged,
            this, [this]() {
        if (m_scene->selectedObjects().count())
            m_propertyEditorDockWidget->setObject(m_scene->selectedObjects().first());
        else
            m_propertyEditorDockWidget->setObject(nullptr);
    });
#else
    connect(m_scene, &SchScene::selectionChanged,
            this, [this]() {
        if (m_scene->selectedObjects().count())
            m_propertyEditorDockWidget->setItem(m_scene->selectedObjects().first());
        else
            m_propertyEditorDockWidget->setItem(nullptr);
    });
#endif
    addArrangeTools();
    addInteractiveTools();
    addSnapTools();
    addPathPointTools();
    addMiscTools();

    Q_INIT_RESOURCE(scheditor);
}

SchEditorWidget::~SchEditorWidget()
{
    qDeleteAll(m_arrangeToolBar->actions());
    qDeleteAll(m_interactiveToolsToolBar->actions());
    qDeleteAll(m_snapToolBar->actions());
    qDeleteAll(m_pathPointToolBar->actions());
    qDeleteAll(m_miscToolBar->actions());
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
    win->addToolBar(m_miscToolBar);
    m_miscToolBar->show();
    win->addToolBar(m_snapToolBar);
    m_snapToolBar->show();
    win->addToolBar(m_arrangeToolBar);
    m_arrangeToolBar->show();
    win->addToolBar(m_pathPointToolBar);
    m_pathPointToolBar->show();
    win->addDockWidget(Qt::LeftDockWidgetArea, m_taskDockWidget);
    m_taskDockWidget->show();
    win->addDockWidget(Qt::RightDockWidgetArea, m_propertyEditorDockWidget);
    m_propertyEditorDockWidget->show();

    m_mainWindow = win;
}

void SchEditorWidget::desactivate(QMainWindow *win)
{
    win->removeDockWidget(m_propertyEditorDockWidget);
    win->removeDockWidget(m_taskDockWidget);
    win->removeToolBar(m_arrangeToolBar);
    win->removeToolBar(m_pathPointToolBar);
    win->removeToolBar(m_snapToolBar);
    win->removeToolBar(m_interactiveToolsToolBar);
    win->removeToolBar(m_miscToolBar);
}

void SchEditorWidget::addInteractiveTools()
{
    m_interactiveToolsActionGroup = new QActionGroup(this);
    m_interactiveToolsToolBar = new QToolBar();

    addInteractiveTool(new GraphicsSelectTool(this));
    addInteractiveTool(new GraphicsLineTool(this));
    //addInteractiveTool(new GraphicsWireTool(this));
    addInteractiveTool(new GraphicsRectTool(this));
    addInteractiveTool(new GraphicsPolygonTool(this));
    addInteractiveTool(new GraphicsCircleTool(this));
    addInteractiveTool(new GraphicsArcTool(this));
    addInteractiveTool(new GraphicsEllipseTool(this));
    addInteractiveTool(new GraphicsBezierTool(this));

#if 0
    QAction *action;
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
        timer->start();
    }
}

void SchEditorWidget::addSnapTools()
{
    m_snapManager = m_view->snapManager(); //new SnapManager(m_view);
    m_snapToolBar = new QToolBar();

    foreach (const QString &group, m_snapManager->groups()) {
        m_snapToolBar->addActions(m_snapManager->actions(group));
    }

    return;
}

void SchEditorWidget::addPathPointTools()
{
    m_pathPointToolBar = new QToolBar();
#if 0
    QAction *action;


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
    m_arrangeToolBar = new QToolBar();
}

void SchEditorWidget::addMiscTools()
{
    m_miscToolBar = new QToolBar();
    m_paletteModeComboBox = new QComboBox;
    m_paletteModeComboBox->addItem("Dark", QVariant::fromValue<Palette::Mode>(Palette::Dark));
    m_paletteModeComboBox->addItem("Light", QVariant::fromValue<Palette::Mode>(Palette::Light));
    m_paletteModeComboBox->setCurrentIndex(0);
    connect(m_paletteModeComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(onPaletteComboBoxIndexChanged(int)));
    m_miscToolBar->addWidget(m_paletteModeComboBox);
}

void SchEditorWidget::onPaletteComboBoxIndexChanged(int index)
{
    m_view->setPaletteMode(m_paletteModeComboBox->itemData(index).value<Palette::Mode>());
}
