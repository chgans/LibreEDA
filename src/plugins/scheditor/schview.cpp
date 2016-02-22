#include "schview.h"
#include "schscene.h"
#include "item/schitem.h"
#include "tool/abstractgraphicsinteractivetool.h"
#include "grid/graphicsgrid.h"
#include "palette.h"
#include "snap/positionsnapper.h"
#include "widget/graphicsviewruler.h"

#include <QGLWidget>
#include <QMouseEvent>
#include <QMessageBox>
#include <QGuiApplication>
#include <QGridLayout>

SchView::SchView(QWidget *parent):
    QGraphicsView(parent),
    m_tool(nullptr),
    m_mousePositionChanged(true),
    m_snapManager(new SnapManager(this)),
    m_snapping(false),
    m_snapToGridEnabled(true),
    m_palette(new Palette(this)),
    m_panning(false),
    m_openGLEnabled(false)
{
    if (m_openGLEnabled)
        setViewport(new QGLWidget);

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
    m_horizontalRuler = new GraphicsViewRuler(GraphicsViewRuler::Horizontal);
    m_verticalRuler = new GraphicsViewRuler(GraphicsViewRuler::Vertical);
    m_cornerWidget = new QWidget;
    m_cornerWidget->setFixedSize(GraphicsViewRuler::BREADTH, GraphicsViewRuler::BREADTH);
    setViewportMargins(GraphicsViewRuler::BREADTH, GraphicsViewRuler::BREADTH, 0, 0);
    layout->addWidget(m_cornerWidget, 0, 0);
    layout->addWidget(m_horizontalRuler, 0, 1);
    layout->addWidget(m_verticalRuler, 1, 0);
    layout->addWidget(viewport(),   1, 1);
    setLayout(layout);
}

SchView::~SchView()
{

}

SchScene *SchView::scene() const
{
    return static_cast<SchScene *>(QGraphicsView::scene());
}

void SchView::setScene(SchScene *scene)
{
    QGraphicsView::setScene(scene);
    applyPalette();
}

AbstractGraphicsInteractiveTool *SchView::tool()
{
    return m_tool;
}

void SchView::setTool(AbstractGraphicsInteractiveTool *tool)
{
    if (m_tool) {
        m_tool->desactivate(m_tool->action(), this);
    }

    m_tool = tool;

    if (m_tool) {
        m_tool->setView(this);
        m_tool->activate(tool->action(), this);
    }
}

SchItem *SchView::objectAt(const QPointF &pos) const
{
    return dynamic_cast<SchItem *>(scene()->itemAt(pos, QTransform()));
}

AbstractGraphicsHandle *SchView::handleAt(const QPointF &pos) const
{
    return dynamic_cast<AbstractGraphicsHandle *>(scene()->itemAt(pos, QTransform()));
}

AbstractGraphicsHandle *SchView::handleUnderMouse() const
{
    return handleAt(mousePosition());
}

SchItem *SchView::objectUnderMouse() const
{
    return objectAt(mousePosition());
}

QPointF SchView::mousePosition() const
{
    return m_mousePosition;
}

QPointF SchView::cursorPosition() const
{
    return m_snapManager->snappedPosition();
}


void SchView::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;

    scale(scaleFactor, scaleFactor);
    updateMousePos();
    updateRulerCursorRanges();
    updateRulerCursorPositions();
}

void SchView::translateView(qreal dx, qreal dy)
{
    translate(dx, dy);
    updateMousePos();
    updateRulerCursorRanges();
    updateRulerCursorPositions();
}

void SchView::setPaletteMode(Palette::Mode mode)
{
    if (mode == m_palette->mode())
        return;
    m_palette->setMode(mode);
    applyPalette();
}

Palette::Mode SchView::paletteMode() const
{
    return m_palette->mode();
}

const Palette *SchView::palette() const
{
    return m_palette;
}

void SchView::drawBackground(QPainter *painter, const QRectF &rect)
{
    painter->fillRect(rect, QBrush(m_palette->backgroundHighlight()));
    painter->fillRect(rect.intersected(sceneRect()), QBrush(m_palette->background()));

    if (scene() && scene()->grid()) {
        scene()->grid()->draw(pixelSize(), painter, rect);
    }
}

void SchView::drawForeground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(rect);

    drawCursor(painter);

    if (m_snapping) {
        drawSnapDecoration(painter);
    }
}

void SchView::wheelEvent(QWheelEvent *event)
{
    if (!event->modifiers().testFlag(Qt::ControlModifier)) {
        // While doing pan view (mid-button), disable wheel event
        // This improve usability for sensitive "vertical wheel" mouses
        // Where you can inadvertly push the wheel sideway while holding
        // the wheel down and the wheel is as well the "mid button"
        if (!event->buttons().testFlag(Qt::MidButton)) {
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

void SchView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MidButton) {
        startPanView(event);
    }
    else if (event->button() == Qt::RightButton) {
        QGraphicsView::mousePressEvent(event);
    }
    else if (m_tool) {
        QMouseEvent ev = createSnappedMouseEvent(event);
        m_tool->mousePressEvent(&ev);
    }
}

void SchView::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons().testFlag(Qt::MidButton)) {
        updatePanView(event);
    }
    else if (m_tool != nullptr) {
        if (m_mousePositionChanged) {
            QMouseEvent ev = createSnappedMouseEvent(event);
            m_tool->mouseMoveEvent(&ev);
        }
    }
    updateMousePos();
    updateRulerCursorPositions();
}

void SchView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MidButton) {
        endPanView();
    }
    else if (m_tool != nullptr) {
        QMouseEvent ev = createSnappedMouseEvent(event);
        m_tool->mouseReleaseEvent(&ev);
    }
}

void SchView::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (m_tool != nullptr)
        m_tool->mouseDoubleClickEvent(event);
    else
        event->ignore();
}

void SchView::keyPressEvent(QKeyEvent *event)
{
    if (m_tool != nullptr)
        m_tool->keyPressEvent(event);
    else
        QGraphicsView::keyPressEvent(event);
}

void SchView::keyReleaseEvent(QKeyEvent *event)
{
    if (m_tool != nullptr)
        m_tool->keyReleaseEvent(event);
    else
        QGraphicsView::keyReleaseEvent(event);
}

void SchView::resizeEvent(QResizeEvent *event)
{
    updateMousePos();
    updateRulerCursorRanges();
    updateRulerCursorPositions();

    QGraphicsView::resizeEvent(event);
}

void SchView::updateMousePos()
{
    QPointF pos = mapToScene(viewport()->mapFromGlobal(QCursor::pos()));
    m_snapping = m_snapManager->snap(pos, 50*transform().m11()); // FIXME: magic number: 50 view's pixels

    if (!m_snapping) {
        m_mousePositionChanged = false;
    }
    else {
        if (m_mousePosition != m_snapManager->snappedPosition()) {
            m_mousePosition = m_snapManager->snappedPosition();
            m_mousePositionChanged = true;
            scene()->update(); // Force redraw of foreground
        }
        else {
            m_mousePositionChanged = false;
        }
    }
}

void SchView::updateRulerCursorRanges()
{
    QPointF topLeft = mapToScene(QPoint(0, 0));
    QPointF bottomRight = mapToScene(QPoint(viewport()->width(),
                                            viewport()->height()));
    m_horizontalRuler->setCursorRange(topLeft.x(), bottomRight.x());
    m_verticalRuler->setCursorRange(topLeft.y(), bottomRight.y());
}

void SchView::updateRulerCursorPositions()
{
    QPointF pos = cursorPosition();
    m_horizontalRuler->setCursorPosition(pos);
    m_verticalRuler->setCursorPosition(pos);
}

QMouseEvent SchView::createSnappedMouseEvent(QMouseEvent *event)
{
    return QMouseEvent(event->type(),
                       m_mousePosition,
                       event->button(),
                       event->buttons(),
                       event->modifiers());
}

void SchView::applyPalette()
{
    m_cornerWidget->setStyleSheet(QString("background-color:%1;").arg(m_palette->backgroundHighlight().name()));
    m_horizontalRuler->setBackgroundColor(m_palette->backgroundHighlight());
    m_verticalRuler->setBackgroundColor(m_palette->backgroundHighlight());
    m_horizontalRuler->setForegroundColor(m_palette->primaryContent());
    m_verticalRuler->setForegroundColor(m_palette->primaryContent());
    scene()->setBackgroundBrush(QBrush(m_palette->background()));
    scene()->grid()->setCoarseLineColor(m_palette->secondaryContent());
    scene()->grid()->setFineLineColor(m_palette->secondaryContent());
    update();
}

void SchView::drawCursor(QPainter *painter)
{
    QPointF pos = cursorPosition();
    QRectF rect = mapToScene(geometry()).boundingRect();
    QPointF top(pos.x(),
                rect.top());
    QPointF bottom(pos.x(),
                   rect.bottom());
    QPointF right(rect.right(),
                  pos.y());
    QPointF left(rect.left(),
                 pos.y());
    painter->setPen(QPen(QBrush(m_palette->emphasisedContent()), 0, Qt::DashLine));
    painter->drawLine(top, bottom);
    painter->drawLine(left, right);
}

void SchView::drawSnapDecoration(QPainter *painter)
{
    // TODO: maybe let the snapmanager paint the decoration:
    // m_snapManager->renderDecoration(painter, pos);
    painter->save();
    painter->translate(cursorPosition());
    painter->scale(1.0/transform().m11(), 1.0/transform().m22());
    painter->setPen(Qt::NoPen);
    painter->setBrush(QBrush(m_palette->emphasisedContent()));
    painter->setRenderHint(QPainter::TextAntialiasing); // Doesn't do anything!
    painter->drawPath(m_snapManager->decoration());
    painter->restore();
}

QSizeF SchView::pixelSize() const
{
    return QSizeF(transform().m11(), transform().m22());
}

SnapManager *SchView::snapManager()
{
    return m_snapManager;
}

void SchView::zoomIn(QPointF pos, qreal factor)
{
    QPoint viewPos = mapFromScene(pos);
    scaleView(factor);
    pos -= mapToScene(viewPos);
    translateView(-pos.x(), -pos.y());
}

void SchView::startPanView(QMouseEvent *event)
{
    // TODO: set cursor icon to closed hand
    m_panning = true;
    m_lastGlobalPos = event->globalPos();
}

void SchView::updatePanView(QMouseEvent *event)
{
    QPoint globalPos = event->globalPos();
    QPointF p1 = mapToScene(mapFromGlobal(globalPos));
    QPointF p2 = mapToScene(mapFromGlobal(m_lastGlobalPos));
    QPointF delta = (p1 - p2);
    translateView(delta.x(), delta.y());
    m_lastGlobalPos = globalPos;
}

void SchView::endPanView()
{
    m_panning = false;
}

