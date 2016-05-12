#include "scheditor.h"
#include "scheditordocument.h"
#include "schview.h"
#include "schscene.h"
#include "item/schitem.h"
#include "scheditorsettings.h"

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

#include "dock/taskdockwidget.h"
#include "dock/propertyeditordockwidget.h"


#include "core/core.h"

#include <QMainWindow>
#include <QAction>
#include <QToolBar>

#include <QFileInfo>
#include <QTimer>

SchEditor::SchEditor(QObject *parent) :
    IEditor(parent)
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

SchEditor::~SchEditor()
{
    qDeleteAll(m_pathPointToolBar->actions());
    qDeleteAll(m_snapToolBar->actions());
    qDeleteAll(m_interactiveToolsToolBar->actions());
    qDeleteAll(m_arrangeToolBar->actions());
    delete m_propertyEditorDockWidget;
    delete m_taskDockWidget;
    delete m_view;
    delete m_scene;
    delete m_document;
}

void SchEditor::addView()
{
    m_view = new SchView();
    m_view->setScene(m_scene);
    m_view->fitInView(m_scene->sceneRect(), Qt::KeepAspectRatio);
}

void SchEditor::addScene()
{
    m_scene = new SchScene(this);
    m_scene->setSceneRect(0, 0, 297, 210);
}

void SchEditor::loadSettings()
{
    /* TODO: dispatch loadSettings to scene, view and tools */
    SchEditorSettings settings;
    settings.load(Core::settings());

    m_view->setPaletteMode(settings.colorScheme);
    m_view->setRulerEnabled(settings.rulerEnabled);
    m_view->setGridEnabled(settings.gridEnabled);
    m_view->setMinimalGridSize(int(settings.minimalGridSize)); // FIXME: int vs uint
    m_view->setGridCoarseMultiplier(int(settings.coarseGridMultiplier));
    if (settings.solidCoarseGridLinesEnabled)
    {
        m_view->setGridCoarseLineStyle(Qt::SolidLine);
    }
    else
    {
        m_view->setGridCoarseLineStyle(Qt::DotLine);
    }
    if (settings.solidFineGridLinesEnabled)
    {
        m_view->setGridFineLineStyle(Qt::SolidLine);
    }
    else
    {
        m_view->setGridFineLineStyle(Qt::DotLine);
    }

    if (!settings.scrollBarsEnabled)
    {
        m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }
    else if (settings.scrollBarsAsNeededEnabled)
    {
        m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    }
    else
    {
        m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    }
    if (!settings.cursorCrosshairEnabled)
    {
        m_view->setMouseCursor(SchView::NoMouseCursor);
    }
    else if (settings.largeCursorCrosshairEnabled)
    {
        m_view->setMouseCursor(SchView::LargeMouseCursor);
    }
    else
    {
        m_view->setMouseCursor(SchView::SmallMouseCursor);
    }
    if (!settings.originCrosshairEnabled)
    {
        m_view->setOriginMark(SchView::NoOriginMark);
    }
    else if (settings.largeOriginCrosshairEnabled)
    {
        m_view->setOriginMark(SchView::LargeOriginMark);
    }
    else
    {
        m_view->setOriginMark(SchView::SmallOriginMark);
    }
    m_view->setRenderHint(QPainter::Antialiasing, settings.antiAliasingEnabled);
    m_view->setRenderHint(QPainter::TextAntialiasing, settings.antiAliasingEnabled);
    m_view->setRenderHint(QPainter::HighQualityAntialiasing, settings.antiAliasingEnabled);
    m_view->setHardwareAccelerationEnabled(settings.hardwareAccelerationEnabled);
}


bool SchEditor::open(QString *errorString, const QString &fileName)
{
    m_document = new SchEditorDocument();
    m_document->setFilePath(fileName);
    QFileInfo fileInfo(fileName);
    m_document->setDisplayName(fileInfo.baseName());
    bool result = m_document->load(errorString, m_document->filePath());
    if (!result)
        return false;
//    foreach (SchItem *item, m_document->items()) {
//        m_scene->addItem(item);
//    }
    return true;
}

IDocument *SchEditor::document() const
{
    return m_document;
}

QIcon SchEditor::icon() const
{
    return QIcon(":/icons/sch.png");
}

QString SchEditor::displayName() const
{
    return m_document->displayName();
}

void SchEditor::activate(QMainWindow *win)
{
    win->addToolBar(m_interactiveToolsToolBar);
    m_interactiveToolsToolBar->show();
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
}

void SchEditor::desactivate(QMainWindow *win)
{
    win->removeDockWidget(m_propertyEditorDockWidget);
    win->removeDockWidget(m_taskDockWidget);
    win->removeToolBar(m_arrangeToolBar);
    win->removeToolBar(m_pathPointToolBar);
    win->removeToolBar(m_snapToolBar);
    win->removeToolBar(m_interactiveToolsToolBar);
}

void SchEditor::addInteractiveTools()
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

void SchEditor::addInteractiveTool(AbstractGraphicsInteractiveTool *tool)
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

void SchEditor::addSnapTools()
{
    m_snapManager = m_view->snapManager(); //new SnapManager(m_view);
    m_snapToolBar = new QToolBar();

    foreach (const QString &group, m_snapManager->groups()) {
        m_snapToolBar->addActions(m_snapManager->actions(group));
    }

    return;
}

void SchEditor::addPathPointTools()
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
void SchEditor::addArrangeTools()
{
    // group/ungroup
    // send to back. front, raise, lower
    // align and distribute
    //
    m_arrangeToolBar = new QToolBar();
}

void SchEditor::addDockWidgets()
{
    m_taskDockWidget = new TaskDockWidget();
    m_propertyEditorDockWidget = new PropertyEditorDockWidget();
#if 1
    connect(m_scene, &SchScene::selectionChanged,
            this, [this]() {
        QList<QObject *> objects;
        foreach (QObject *object, m_scene->selectedObjects()) {
            objects.append(object);
        }
        m_propertyEditorDockWidget->setObjects(objects);
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
}
