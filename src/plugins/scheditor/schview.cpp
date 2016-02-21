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

//#define NO_EVENT

// Tools:
//  Select (default)
//  ZoomBox
//  Drag
//  Insert
//  Edit
// Use graphical effect for item selection and move ?
// createPhantom() and createClone() to help the model command stuff

/* TODO:
 *  - context menu:
 *   - tool options
 *   - cancel
 *   - properties
 */

SchView::SchView(QWidget *parent):
    QGraphicsView(parent),
    m_tool(nullptr),
    m_objectUnderMouse(nullptr),
    m_handleUnderMouse(nullptr),
    m_mousePositionChanged(true),
    m_snapToGridEnabled(true),
    m_palette(new Palette(this)),
    m_snapManager(new SnapManager(this)),
    m_snapping(false)
{    
    //setViewport(new QGLWidget);

    // The widget emits the QWidget::customContextMenuRequested() signal
    setContextMenuPolicy(Qt::CustomContextMenu);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setDragMode(QGraphicsView::NoDrag);
    setMouseTracking(true);

    // AnchorUnderMouse doesn't work for some reason
    // Use NoAnchor and fix it up in scaleView()
    setTransformationAnchor(NoAnchor);
    setResizeAnchor(NoAnchor);
#ifdef NO_EVENT
    setDragMode(RubberBandDrag);
    setTransformationAnchor(AnchorUnderMouse);
    setResizeAnchor(AnchorUnderMouse);
#endif

    QGridLayout *layout = new QGridLayout;
    layout->setSpacing(0);
    layout->setMargin(0);
    m_hViewRuler = new GraphicsViewRuler(GraphicsViewRuler::Horizontal);
    //m_hViewRuler->setMouseTrack(true);
    m_vViewRuler = new GraphicsViewRuler(GraphicsViewRuler::Vertical);
    //m_vViewRuler->setMouseTrack(true);
    QWidget *cornerWidget = new QWidget;
    cornerWidget->setStyleSheet("background-color:#073642;");
    //cornerWidget->setBackgroundRole(QPalette::Window);
    cornerWidget->setFixedSize(GraphicsViewRuler::BREADTH, GraphicsViewRuler::BREADTH);
    setViewportMargins(GraphicsViewRuler::BREADTH, GraphicsViewRuler::BREADTH, 0, 0);
    layout->addWidget(cornerWidget, 0, 0);
    layout->addWidget(m_hViewRuler, 0, 1);
    layout->addWidget(m_vViewRuler, 1, 0);
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

    QPointF topLeft = mapToScene(QPoint(0, 0));
    QPointF bottomRight = mapToScene(QPoint(viewport()->width(),
                                            viewport()->height()));
    m_hViewRuler->setRange(topLeft.x(), bottomRight.x());
    m_vViewRuler->setRange(topLeft.y(), bottomRight.y());
}

void SchView::translateView(qreal dx, qreal dy)
{
    translate(dx, dy);
    QPointF topLeft = mapToScene(QPoint(0, 0));
    QPointF bottomRight = mapToScene(QPoint(viewport()->width(),
                                            viewport()->height()));
    m_hViewRuler->setRange(topLeft.x(), bottomRight.x());
    m_vViewRuler->setRange(topLeft.y(), bottomRight.y());
    updateMousePos();
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
    painter->fillRect(rect, QBrush(m_palette->background2()));
    QGraphicsView::drawBackground(painter, rect);

    // Grid
    if (scene() && scene()->grid()) {
        scene()->grid()->draw(pixelSize(), painter, rect);
    }
}

void SchView::drawForeground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(rect);
    //QGraphicsView::drawForeground(painter, rect);

    QPointF p = mapToScene(mousePosition());

    if (p.isNull())
        return;

    // Cursor: TODO allow to choose cursor
#if 0
    qreal length = 50.0/transform().m11();
    QPointF top(p.x(),
                p.y() - length);
    QPointF bottom(p.x(),
                   p.y() + length);
    QPointF right(p.x() + length,
                  p.y());
    QPointF left(p.x() - length,
                 p.y());
    painter->setPen(QPen(Qt::black, 0, Qt::SolidLine));
    painter->drawLine(top, bottom);
    painter->drawLine(left, right);
#else
    QRectF r = mapToScene(geometry()).boundingRect();
    QPointF top(p.x(),
                r.top());
    QPointF bottom(p.x(),
                   r.bottom());
    QPointF right(r.right(),
                  p.y());
    QPointF left(r.left(),
                 p.y());
    painter->setPen(QPen(QBrush(QColor("#586e75")), 0, Qt::DashLine));
    painter->drawLine(top, bottom);
    painter->drawLine(left, right);
#endif
    if (m_snapping) {
        // TODO: maybe let the snapmanager paint the decoration:
        // m_snapManager->renderDecoration(painter, pos);
        painter->setPen(Qt::NoPen);
        painter->setBrush(QBrush(QColor("#93a1a1")));
        painter->setRenderHint(QPainter::TextAntialiasing); // Doesn't do anything!
        painter->drawPath(mapToScene(m_snapManager->decoration()));
    }
}

// TODO: Zoom here or tool
void SchView::wheelEvent(QWheelEvent *event)
{
#ifdef NO_EVENT
    QGraphicsView::wheelEvent(event);
    return;
#endif
    if (!event->modifiers().testFlag(Qt::ControlModifier)) {
        // While doing pan view (mid-button), disable wheel event
        // This improve usability for sensitive "vertical wheel" mouses
        // Where you can inadvertly push the wheel sideway while holding
        // the wheel down and the wheel is as well the "mid button"
        if (!event->buttons().testFlag(Qt::MidButton)) {
            QGraphicsView::wheelEvent(event);
            QPointF topLeft = mapToScene(QPoint(0, 0));
            QPointF bottomRight = mapToScene(QPoint(viewport()->width(),
                                                    viewport()->height()));
            m_hViewRuler->setRange(topLeft.x(), bottomRight.x());
            m_vViewRuler->setRange(topLeft.y(), bottomRight.y());
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
#ifdef NO_EVENT
    QGraphicsView::mousePressEvent(event);
    return;
#endif

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
        QMouseEvent ev = snapMouseEvent(event);
        m_tool->mousePressEvent(&ev);
    }
    else {
        event->ignore();
    }
}

// FIXME: Don't snap cursor for handles
void SchView::mouseMoveEvent(QMouseEvent *event)
{
#ifdef NO_EVENT
    QGraphicsView::mouseMoveEvent(event);
    return;
#endif

    if (event->buttons().testFlag(Qt::MidButton)) {
        QPoint globalPos = event->globalPos();
        if ((globalPos - m_lastGlobalPos).manhattanLength() > 2) {
            QPointF p1 = mapToScene(mapFromGlobal(globalPos));
            QPointF p2 = mapToScene(mapFromGlobal(m_lastGlobalPos));
            QPointF delta = (p1 - p2);
            translateView(delta.x(), delta.y());
            m_lastGlobalPos = globalPos;
            QPointF pos = mapToScene(mapFromGlobal(event->globalPos()));
            m_hViewRuler->setCursorPosition(pos);
            m_vViewRuler->setCursorPosition(pos);
        }
        event->accept();
        return;
    }

    updateMousePos();

    if (m_tool != nullptr) {
        if (m_mousePositionChanged) {
            QMouseEvent ev = snapMouseEvent(event);
            m_tool->mouseMoveEvent(&ev);
        }
    }
    QPointF pos = mapToScene(mapFromGlobal(event->globalPos()));
    m_hViewRuler->setCursorPosition(pos);
    m_vViewRuler->setCursorPosition(pos);
}

void SchView::mouseReleaseEvent(QMouseEvent *event)
{
#ifdef NO_EVENT
    QGraphicsView::mouseReleaseEvent(event);
    return;
#endif
    if (event->button() == Qt::MidButton) {
        m_panning = false;
        event->accept();
        return;
    }

    updateMousePos();

    if (m_tool != nullptr) {
        QMouseEvent ev = snapMouseEvent(event);
        m_tool->mouseReleaseEvent(&ev);
    }
    else
        event->ignore();
}

void SchView::mouseDoubleClickEvent(QMouseEvent *event)
{
#ifdef NO_EVENT
    QGraphicsView::mouseDoubleClickEvent(event);
    return;
#endif
    if (m_tool != nullptr)
        m_tool->mouseDoubleClickEvent(event);
    else
        event->ignore();
}

void SchView::keyPressEvent(QKeyEvent *event)
{
#ifdef NO_EVENT
    QGraphicsView::keyPressEvent(event);
    return;
#endif

    if (m_tool != nullptr)
        m_tool->keyPressEvent(event);
    else
        QGraphicsView::keyPressEvent(event);
}

void SchView::keyReleaseEvent(QKeyEvent *event)
{
#ifdef NO_EVENT
    QGraphicsView::keyReleaseEvent(event);
    return;
#endif

    if (m_tool != nullptr)
        m_tool->keyReleaseEvent(event);
    else
        QGraphicsView::keyReleaseEvent(event);
}

void SchView::updateMousePos()
{
    QPoint viewPos = mapFromGlobal(QCursor::pos());
    m_snapping = m_snapManager->snap(viewPos, 50);

    if (!m_snapping) {
        m_mousePositionChanged = false;
    }
    else {
        // TODO: highlight snapped items
        if (m_mousePosition != m_snapManager->snappedPosition()) {
            m_mousePosition = m_snapManager->snappedPosition();
            m_mousePositionChanged = true;
            scene()->update(); // Force redraw of foreground
        }
        else {
            m_mousePositionChanged = false;
        }
    }
    qDebug() << "Mouse position:" << mapToScene(viewPos);
    qDebug() << "Cursor position:" << mapToScene(m_mousePosition);
}

QMouseEvent SchView::snapMouseEvent(QMouseEvent *event)
{
    return QMouseEvent(event->type(),
                       m_mousePosition,
                       event->button(),
                       event->buttons(),
                       event->modifiers());
}

void SchView::applyPalette()
{
    scene()->setBackgroundBrush(QBrush(m_palette->background1()));
    scene()->grid()->setCoarseLineColor(QColor("#839496"));
    scene()->grid()->setFineLineColor(QColor("#586e75"));
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


void SchView::resizeEvent(QResizeEvent *event)
{
    updateMousePos();

    QPointF topLeft = mapToScene(QPoint(0, 0));
    QPointF bottomRight = mapToScene(QPoint(viewport()->width(),
                                            viewport()->height()));
    m_hViewRuler->setRange(topLeft.x(), bottomRight.x());
    m_vViewRuler->setRange(topLeft.y(), bottomRight.y());
    QPointF pos = mapToScene(mapFromGlobal(QCursor::pos()));
    m_hViewRuler->setCursorPosition(pos);

    QGraphicsView::resizeEvent(event);
}
