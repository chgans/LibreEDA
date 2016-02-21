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
    m_objectUnderMouse(nullptr),
    m_handleUnderMouse(nullptr),
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

SchScene *SchView::scene()
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

SchItem *SchView::objectAt(const QPoint &pos) const
{
    return dynamic_cast<SchItem *>(QGraphicsView::itemAt(pos));
}

AbstractGraphicsHandle *SchView::handleAt(const QPoint &pos) const
{
    return dynamic_cast<AbstractGraphicsHandle *>(QGraphicsView::itemAt(pos));
}

AbstractGraphicsHandle *SchView::handleUnderMouse() const
{
    return handleAt(mousePosition());
}

SchItem *SchView::objectUnderMouse() const
{
    return objectAt(mousePosition());
}

QPoint SchView::mousePosition() const
{
    return m_mousePosition;
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

    QPointF p = mapToScene(mousePosition());

    if (p.isNull())
        return;

    QRectF r = mapToScene(geometry()).boundingRect();
    QPointF top(p.x(),
                r.top());
    QPointF bottom(p.x(),
                   r.bottom());
    QPointF right(r.right(),
                  p.y());
    QPointF left(r.left(),
                 p.y());
    painter->setPen(QPen(QBrush(m_palette->emphasisedContent()), 0, Qt::DashLine));
    painter->drawLine(top, bottom);
    painter->drawLine(left, right);

    if (m_snapping) {
        // TODO: maybe let the snapmanager paint the decoration:
        // m_snapManager->renderDecoration(painter, pos);
        painter->setPen(Qt::NoPen);
        painter->setBrush(QBrush(m_palette->emphasisedContent()));
        painter->setRenderHint(QPainter::TextAntialiasing); // Doesn't do anything!
        painter->drawPath(mapToScene(m_snapManager->decoration()));
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
    scaleView(pow((double)2, -event->delta() / 240.0));
    pos -= mapToScene(event->pos());
    translateView(-pos.x(), -pos.y());

    event->accept();
}

void SchView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MidButton) {
        m_panning = true;
        m_lastGlobalPos = event->globalPos();
        event->accept();
        return;
    }

    updateMousePos();

    if (event->button() == Qt::RightButton) {
        QGraphicsView::mousePressEvent(event);
    }
    else if (m_tool) {
        QMouseEvent ev = createSnappedMouseEvent(event);
        m_tool->mousePressEvent(&ev);
    }
    else {
        event->ignore();
    }
}

void SchView::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons().testFlag(Qt::MidButton)) {
        QPoint globalPos = event->globalPos();
        if ((globalPos - m_lastGlobalPos).manhattanLength() > 2) {
            QPointF p1 = mapToScene(mapFromGlobal(globalPos));
            QPointF p2 = mapToScene(mapFromGlobal(m_lastGlobalPos));
            QPointF delta = (p1 - p2);
            translateView(delta.x(), delta.y());
            m_lastGlobalPos = globalPos;
            updateRulerCursorPositions();
        }
        event->accept();
        return;
    }

    updateMousePos();

    if (m_tool != nullptr) {
        if (m_mousePositionChanged) {
            QMouseEvent ev = createSnappedMouseEvent(event);
            m_tool->mouseMoveEvent(&ev);
        }
    }
    updateRulerCursorPositions();
}

void SchView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MidButton) {
        m_panning = false;
        event->accept();
        return;
    }

    updateMousePos();

    if (m_tool != nullptr) {
        QMouseEvent ev = createSnappedMouseEvent(event);
        m_tool->mouseReleaseEvent(&ev);
    }
    else
        event->ignore();
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
    QPoint viewPos = viewport()->mapFromGlobal(QCursor::pos());
    m_snapping = m_snapManager->snap(viewPos, 50);

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
    QPointF pos = mapToScene(viewport()->mapFromGlobal(QCursor::pos()));
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

QSizeF SchView::pixelSize() const
{
    return QSizeF(transform().m11(), transform().m22());
}

SnapManager *SchView::snapManager()
{
    return m_snapManager;
}

