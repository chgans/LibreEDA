#include "view.h"
#include "scene.h"
#include "item/item.h"
#include "tool/interactivetool.h"
#include "grid/cartesiangrid.h"
#include "palette.h"
#include "snap/positionsnapper.h"
#include "widget/rulerbarwidget.h"
#include "settings.h"

#include <QOpenGLWidget>
#include <QMouseEvent>
#include <QMessageBox>
#include <QGuiApplication>
#include <QGridLayout>

using namespace SymbolEditor;

// FIXME: Private NS
static const int OriginMarkPixelSize = 10;
static const int CursorMarkPixelSize = 10;

View::View(QWidget *parent):
    QGraphicsView(parent),
    m_grid(new CartesianGrid()),
    m_tool(nullptr),
    m_mousePositionChanged(true),
    m_snapManager(new SnapManager(this)),
    m_snapping(false),
    m_snapToGridEnabled(true),
    m_palette(new Palette(this)),
    m_panning(false)
{

    setContextMenuPolicy(Qt::CustomContextMenu);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setDragMode(QGraphicsView::NoDrag);
    setMouseTracking(true);
    setTransformationAnchor(NoAnchor);
    setResizeAnchor(NoAnchor);

    QGridLayout *layout = new QGridLayout;
    layout->setSpacing(0);
    layout->setMargin(0);
    m_horizontalRuler = new RulerBarWidget(RulerBarWidget::Horizontal);
    m_verticalRuler = new RulerBarWidget(RulerBarWidget::Vertical);
    m_cornerWidget = new QWidget;
    m_cornerWidget->setFixedSize(RulerBarWidget::BREADTH, RulerBarWidget::BREADTH);
    layout->addWidget(viewport(),   1, 1);
    setLayout(layout);

    setHardwareAccelerationEnabled(false);
    setRulerEnabled(true);
    setGridEnabled(true);
    setMinimalGridSize(10);
    setGridCoarseMultiplier(10);
    setGridCoarseLineStyle(Qt::SolidLine);
    setGridFineLineStyle(Qt::DotLine);
    setMouseCursor(LargeMouseCursor);
    setOriginMark(LargeOriginMark);
    setPaletteMode(Palette::Dark);
}

View::~View()
{

}

Scene *View::scene() const
{
    return static_cast<Scene *>(QGraphicsView::scene());
}

void View::setScene(Scene *scene)
{
    QGraphicsView::setScene(scene);
    QRectF rect = scene->sceneRect();
    m_grid->setOrigin(rect.topLeft());
    m_grid->setSize(rect.size());
    m_grid->setStep(QPointF(10, 10));
    applyPalette();
}

InteractiveTool *View::tool()
{
    return m_tool;
}

void View::setTool(InteractiveTool *tool)
{
    m_tool = tool;

    if (m_tool)
    {
        m_tool->setView(this);
    }
}

Item *View::objectAt(const QPointF &pos) const
{
    return dynamic_cast<Item *>(scene()->itemAt(pos, QTransform()));
}

Handle *View::handleAt(const QPointF &pos) const
{
    return dynamic_cast<Handle *>(scene()->itemAt(pos, QTransform()));
}

Handle *View::handleUnderMouse() const
{
    return handleAt(mousePosition());
}

Item *View::objectUnderMouse() const
{
    return objectAt(mousePosition());
}

QPointF View::mousePosition() const
{
    return m_mousePosition;
}

QPointF View::cursorPosition() const
{
    return m_snapManager->snappedPosition();
}

QPointF View::originPosition() const
{
    return QPointF(0, 0);
}

void View::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
    {
        return;
    }

    scale(scaleFactor, scaleFactor);
    updateMousePos();
    updateRulerCursorRanges();
    updateRulerCursorPositions();
}

void View::translateView(qreal dx, qreal dy)
{
    translate(dx, dy);
    updateMousePos();
    updateRulerCursorRanges();
    updateRulerCursorPositions();
}

void View::setPaletteMode(Palette::Mode mode)
{
    if (mode == m_palette->mode())
    {
        return;
    }
    m_palette->setMode(mode);
    applyPalette();
}

Palette::Mode View::paletteMode() const
{
    return m_palette->mode();
}

const Palette *View::palette() const
{
    return m_palette;
}

void View::drawBackground(QPainter *painter, const QRectF &rect)
{
    painter->fillRect(rect, QBrush(m_palette->backgroundHighlight()));
    painter->fillRect(rect.intersected(sceneRect()), QBrush(m_palette->background()));

    if (m_gridEnabled)
    {
        m_grid->draw(pixelSize(), painter, rect);
    }
}

void View::drawForeground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(rect);

    drawCursor(painter);
    drawOrigin(painter);

    if (m_snapping)
    {
        drawSnapDecoration(painter);
    }
}

void View::wheelEvent(QWheelEvent *event)
{
    if (!event->modifiers().testFlag(Qt::ControlModifier))
    {
        // While doing pan view (mid-button), disable wheel event
        // This improve usability for sensitive "vertical wheel" mouses
        // Where you can inadvertly push the wheel sideway while holding
        // the wheel down and the wheel is as well the "mid button"
        if (!event->buttons().testFlag(Qt::MidButton))
        {
            QGraphicsView::wheelEvent(event);
            updateRulerCursorRanges();
            updateRulerCursorPositions();
        }
        return;
    }

    QPointF pos = mapToScene(event->pos());
    qreal factor = pow((double)2, -event->delta() / 240.0);
    zoomIn(pos, factor);
}

void View::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MidButton)
    {
        startPanView(event);
    }
    else if (event->button() == Qt::RightButton)
    {
        QGraphicsView::mousePressEvent(event);
    }
    else if (m_tool)
    {
        QMouseEvent ev = createSnappedMouseEvent(event);
        m_tool->mousePressEvent(&ev);
    }
}

void View::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons().testFlag(Qt::MidButton))
    {
        updatePanView(event);
    }
    else if (m_tool != nullptr)
    {
        if (m_mousePositionChanged)
        {
            QMouseEvent ev = createSnappedMouseEvent(event);
            m_tool->mouseMoveEvent(&ev);
        }
    }
    updateMousePos();
    updateRulerCursorPositions();
    updateForeground();
}

void View::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MidButton)
    {
        endPanView();
    }
    else if (m_tool != nullptr)
    {
        QMouseEvent ev = createSnappedMouseEvent(event);
        m_tool->mouseReleaseEvent(&ev);
    }
}

void View::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (m_tool != nullptr)
    {
        m_tool->mouseDoubleClickEvent(event);
    }
    else
    {
        event->ignore();
    }
}

void View::keyPressEvent(QKeyEvent *event)
{
    if (m_tool != nullptr)
    {
        m_tool->keyPressEvent(event);
    }
    else
    {
        QGraphicsView::keyPressEvent(event);
    }
}

void View::keyReleaseEvent(QKeyEvent *event)
{
    if (m_tool != nullptr)
    {
        m_tool->keyReleaseEvent(event);
    }
    else
    {
        QGraphicsView::keyReleaseEvent(event);
    }
}

void View::resizeEvent(QResizeEvent *event)
{
    QGraphicsView::resizeEvent(event);
    updateMousePos();
    updateRulerCursorRanges();
    updateRulerCursorPositions();
}

void View::updateMousePos()
{
    QPointF pos = mapToScene(viewport()->mapFromGlobal(QCursor::pos()));
    m_snapping = m_snapManager->snap(pos,
                                     50 * transform().m11()); // FIXME: magic number: 50 view's pixels

    if (!m_snapping)
    {
        m_mousePositionChanged = false;
    }
    else
    {
        if (m_mousePosition != m_snapManager->snappedPosition())
        {
            m_mousePosition = m_snapManager->snappedPosition();
            m_mousePositionChanged = true;
        }
        else
        {
            m_mousePositionChanged = false;
        }
    }
}

void View::updateRulerCursorRanges()
{
    QPointF topLeft = mapToScene(QPoint(0, 0));
    QPointF bottomRight = mapToScene(QPoint(m_horizontalRuler->width(),
                                            m_verticalRuler->height()));
    m_horizontalRuler->setCursorRange(topLeft.x(), bottomRight.x());
    m_verticalRuler->setCursorRange(topLeft.y(), bottomRight.y());
}

void View::updateRulerCursorPositions()
{
    QPointF pos = cursorPosition();
    m_horizontalRuler->setCursorPosition(pos);
    m_verticalRuler->setCursorPosition(pos);
}

QMouseEvent View::createSnappedMouseEvent(QMouseEvent *event)
{
    return QMouseEvent(event->type(),
                       m_mousePosition,
                       event->button(),
                       event->buttons(),
                       event->modifiers());
}

QRectF View::visibleSceneRect() const
{
    QPointF topLeft = mapToScene(QPoint(0, 0));
    QPointF bottomRight = mapToScene(QPoint(viewport()->width(), viewport()->height()));
    return QRectF(topLeft, bottomRight);
}

void View::updateForeground()
{
    invalidateScene(visibleSceneRect(), QGraphicsScene::ForegroundLayer);
    viewport()->update();
}

void View::updateBackground()
{
    invalidateScene(visibleSceneRect(), QGraphicsScene::BackgroundLayer);
    viewport()->update();
}

void View::applyPalette()
{
    m_cornerWidget->setStyleSheet(QString("background-color:%1;").arg(
                                      m_palette->backgroundHighlight().name()));
    m_horizontalRuler->setBackgroundColor(m_palette->backgroundHighlight());
    m_verticalRuler->setBackgroundColor(m_palette->backgroundHighlight());
    m_horizontalRuler->setForegroundColor(m_palette->primaryContent());
    m_verticalRuler->setForegroundColor(m_palette->primaryContent());
    scene()->setBackgroundBrush(QBrush(m_palette->background()));
    m_grid->setCoarseLineColor(m_palette->secondaryContent());
    m_grid->setFineLineColor(m_palette->secondaryContent());
    update();
}

void View::drawCursor(QPainter *painter)
{
    if (mouseCursor() == NoMouseCursor)
    {
        return;
    }

    QPointF pos = cursorPosition();
    painter->setPen(QPen(m_palette->orange(), 0, Qt::SolidLine));
    QRectF rect(0, 0,
                CursorMarkPixelSize / transform().m11(), CursorMarkPixelSize / transform().m22());
    rect.moveCenter(pos);
    painter->drawEllipse(rect);

    painter->setPen(QPen(m_palette->orange(), 0, Qt::DashLine));
    if (mouseCursor() == LargeMouseCursor)
    {
        rect = visibleSceneRect();
    }
    else
    {
        rect = QRectF(0, 0,
                      4 * CursorMarkPixelSize / transform().m11(),
                      4 * CursorMarkPixelSize / transform().m22());
        rect.moveCenter(pos);
    }
    QPointF top(pos.x(),
                rect.top());
    QPointF bottom(pos.x(),
                   rect.bottom());
    QPointF right(rect.right(),
                  pos.y());
    QPointF left(rect.left(),
                 pos.y());
    painter->drawLine(top, bottom);
    painter->drawLine(left, right);
}

void View::drawOrigin(QPainter *painter)
{
    if (originMark() == NoOriginMark)
    {
        return;
    }

    QPointF pos = originPosition();

    painter->setPen(QPen(m_palette->red(), 0, Qt::SolidLine));
    QRectF rect(0, 0,
                OriginMarkPixelSize / transform().m11(), OriginMarkPixelSize / transform().m22());
    rect.moveCenter(pos);
    painter->drawEllipse(rect);

    painter->setPen(QPen(m_palette->red(), 0, Qt::DashDotLine));
    if (originMark() == LargeOriginMark)
    {
        rect = visibleSceneRect();
    }
    else
    {
        rect = QRectF(0, 0,
                      4 * OriginMarkPixelSize / transform().m11(),
                      4 * OriginMarkPixelSize / transform().m22());
        rect.moveCenter(pos);
    }
    QPointF top(pos.x(),
                rect.top());
    QPointF bottom(pos.x(),
                   rect.bottom());
    QPointF right(rect.right(),
                  pos.y());
    QPointF left(rect.left(),
                 pos.y());
    painter->drawLine(top, bottom);
    painter->drawLine(left, right);
}

void View::drawSnapDecoration(QPainter *painter)
{
    // TODO: maybe let the snapmanager paint the decoration:
    // m_snapManager->renderDecoration(painter, pos);
    painter->save();
    painter->translate(cursorPosition());
    painter->scale(1.0 / transform().m11(), 1.0 / transform().m22());
    painter->setPen(Qt::NoPen);
    painter->setBrush(QBrush(m_palette->emphasisedContent()));
    painter->setRenderHint(QPainter::Antialiasing);
    painter->drawPath(m_snapManager->decoration());
    painter->restore();
}

QSizeF View::pixelSize() const
{
    return QSizeF(transform().m11(), transform().m22());
}

const Grid *View::grid() const
{
    return m_grid;
}

SnapManager *View::snapManager()
{
    return m_snapManager;
}

void View::zoomIn(QPointF pos, qreal factor)
{
    QPoint viewPos = mapFromScene(pos);
    scaleView(factor);
    pos -= mapToScene(viewPos);
    translateView(-pos.x(), -pos.y());
}

void View::setHardwareAccelerationEnabled(bool enabled)
{
    if (m_hardwareAccelerationEnabled == enabled)
    {
        return;
    }

    // Breaks mouse tracking, maybe need to call setMouseTracking after
    // setViewPort, and maybe even more things....
#if 0
    if (m_hardwareAccelerationEnabled)
    {
        setViewport(new QOpenGLWidget);
    }
    else
    {
        setViewport(new QWidget);
    }
#else
    return;
#endif
}

bool View::hardwareAccelerationEnabled() const
{
    return m_hardwareAccelerationEnabled;
}

void View::setRulerEnabled(bool enabled)
{
    if (m_rulerEnabled == enabled)
    {
        return;
    }

    m_rulerEnabled = enabled;

    if (m_rulerEnabled)
    {
        QGridLayout *grid = static_cast<QGridLayout *>(layout());
        grid->addWidget(m_cornerWidget, 0, 0);
        m_cornerWidget->show();
        grid->addWidget(m_horizontalRuler, 0, 1);
        m_horizontalRuler->show();
        grid->addWidget(m_verticalRuler, 1, 0);
        m_verticalRuler->show();
        setViewportMargins(RulerBarWidget::BREADTH, RulerBarWidget::BREADTH, 0, 0);
    }
    else
    {
        QGridLayout *grid = static_cast<QGridLayout *>(layout());
        grid->removeWidget(m_cornerWidget);
        m_cornerWidget->hide();
        grid->removeWidget(m_horizontalRuler);
        m_horizontalRuler->hide();
        grid->removeWidget(m_verticalRuler);
        m_verticalRuler->hide();
        setViewportMargins(0, 0, 0, 0);
    }
}

bool View::rulerEnabled() const
{
    return m_rulerEnabled;
}

void View::setGridEnabled(bool enabled)
{
    if (m_gridEnabled == enabled)
    {
        return;
    }
    m_gridEnabled = enabled;
    updateBackground();
}

bool View::gridEnabled() const
{
    return m_gridEnabled;
}

void View::setMinimalGridSize(int pixels)
{
    m_grid->setMinimalFeatureSize(pixels);
    updateBackground();
}

int View::minimalGridSize() const
{
    return m_grid->minimalFeatureSize();
}

void View::setGridCoarseMultiplier(int multiplier)
{
    m_grid->setCoarseMultiplier(multiplier);
    updateBackground();
}

int View::gridCoarseMultiplier() const
{
    return m_grid->coarseMultiplier();
}

void View::setGridCoarseLineStyle(Qt::PenStyle style)
{
    m_grid->setCoarseLineStyle(style);
    updateBackground();
}

Qt::PenStyle View::gridCoarseLineStyle() const
{
    return m_grid->coarseLineStyle();
}

void View::setGridFineLineStyle(Qt::PenStyle style)
{
    m_grid->setFineLineStyle(style);
    updateBackground();
}

Qt::PenStyle View::gridFineLineStyle() const
{
    return m_grid->fineLineStyle();
}

void View::setMouseCursor(View::MouseCursor cursor)
{
    if (m_mouseCursor == cursor)
    {
        return;
    }
    m_mouseCursor = cursor;
    updateForeground();
}

View::MouseCursor View::mouseCursor() const
{
    return m_mouseCursor;
}

void View::setOriginMark(View::OriginMark mark)
{
    if (m_originMark == mark)
    {
        return;
    }
    m_originMark = mark;
    updateForeground();
}

View::OriginMark View::originMark() const
{
    return m_originMark;
}

void View::applySettings(const Settings &settings)
{
    setPaletteMode(settings.colorScheme);
    setRulerEnabled(settings.rulerEnabled);
    setGridEnabled(settings.gridEnabled);
    setMinimalGridSize(int(settings.minimalGridSize)); // FIXME: int vs uint
    setGridCoarseMultiplier(int(settings.coarseGridMultiplier));
    if (settings.solidCoarseGridLinesEnabled)
    {
        setGridCoarseLineStyle(Qt::SolidLine);
    }
    else
    {
        setGridCoarseLineStyle(Qt::DotLine);
    }
    if (settings.solidFineGridLinesEnabled)
    {
        setGridFineLineStyle(Qt::SolidLine);
    }
    else
    {
        setGridFineLineStyle(Qt::DotLine);
    }

    if (!settings.scrollBarsEnabled)
    {
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }
    else if (settings.scrollBarsAsNeededEnabled)
    {
        setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    }
    else
    {
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    }
    if (!settings.cursorCrosshairEnabled)
    {
        setMouseCursor(NoMouseCursor);
    }
    else if (settings.largeCursorCrosshairEnabled)
    {
        setMouseCursor(LargeMouseCursor);
    }
    else
    {
        setMouseCursor(SmallMouseCursor);
    }
    if (!settings.originCrosshairEnabled)
    {
        setOriginMark(NoOriginMark);
    }
    else if (settings.largeOriginCrosshairEnabled)
    {
        setOriginMark(LargeOriginMark);
    }
    else
    {
        setOriginMark(SmallOriginMark);
    }
    setRenderHint(QPainter::Antialiasing, settings.antiAliasingEnabled);
    setRenderHint(QPainter::TextAntialiasing, settings.antiAliasingEnabled);
    setRenderHint(QPainter::HighQualityAntialiasing, settings.antiAliasingEnabled);
    setHardwareAccelerationEnabled(settings.hardwareAccelerationEnabled);

}

void View::startPanView(QMouseEvent *event)
{
    // TODO: set cursor icon to closed hand
    m_panning = true;
    m_lastGlobalPos = event->globalPos();
}

void View::updatePanView(QMouseEvent *event)
{
    QPoint globalPos = event->globalPos();
    QPointF p1 = mapToScene(mapFromGlobal(globalPos));
    QPointF p2 = mapToScene(mapFromGlobal(m_lastGlobalPos));
    QPointF delta = (p1 - p2);
    translateView(delta.x(), delta.y());
    m_lastGlobalPos = globalPos;
}

void View::endPanView()
{
    m_panning = false;
}

