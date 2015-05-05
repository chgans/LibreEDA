#include "schview.h"
#include "schscene.h"
#include "schitem.h"
#include "abstractgraphicsinteractivetool.h"
#include "graphicsgrid.h"
#include "palette.h"

#include <QGLWidget>

#include <QMouseEvent>
#include <QMessageBox>

#include <QDebug>

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
    m_palette(new Palette(this))
{    
    setViewport(new QGLWidget);

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
}

void SchView::setPaletteMode(Palette::Mode mode)
{
    if (mode == m_palette->mode())
        return;
    m_palette->setMode(mode);
    scene()->setBackgroundBrush(QBrush(m_palette->background1()));
    scene()->grid()->setCoarseLineColor(m_palette->content6());
    scene()->grid()->setFineLineColor(m_palette->content3());
    update();
}

Palette::Mode SchView::paletteMode() const
{
    return m_palette->mode();
}

const Palette *SchView::palette() const
{
    return m_palette;
}

void SchView::enableSnapToGrid(bool enabled)
{
    m_snapToGridEnabled = enabled;
}

bool SchView::isSnapToGridEnabled() const
{
    return m_snapToGridEnabled;
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

    // Cursor
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

}

// TODO: Zoom here or tool
void SchView::wheelEvent(QWheelEvent *event)
{
    if (!event->modifiers().testFlag(Qt::ControlModifier)) {
        QGraphicsView::wheelEvent(event);
        return;
    }
    QPointF pos = mapToScene(event->pos());
    scaleView(pow((double)2, -event->delta() / 240.0));
    pos -= mapToScene(event->pos());
    translate(-pos.x(), -pos.y());

    event->accept();
}

void SchView::mousePressEvent(QMouseEvent *event)
{
    updateMousePos();

    if (event->button() == Qt::RightButton) {
        QGraphicsView::mousePressEvent(event);
    }
    else if (event->button() == Qt::LeftButton && m_tool) {
        QMouseEvent ev = snapMouseEvent(event);
        m_tool->mousePressEvent(&ev);
    }
    else {
        event->ignore();
    }
}

void SchView::mouseMoveEvent(QMouseEvent *event)
{
    updateMousePos();

    if (objectUnderMouse() && !m_objectUnderMouse) {
        m_objectUnderMouse = objectUnderMouse();
    }
    if (!objectUnderMouse() && m_objectUnderMouse) {
        m_objectUnderMouse = nullptr;
    }
    if (handleUnderMouse() && !m_handleUnderMouse) {
        m_handleUnderMouse = handleUnderMouse();
    }
    if (!handleUnderMouse() && m_handleUnderMouse) {
        m_handleUnderMouse = nullptr;
    }

    if (m_tool != nullptr) {
        if (m_mousePositionChanged) {
            QMouseEvent ev = snapMouseEvent(event);
            m_tool->mouseMoveEvent(&ev);
        }
    }
    else
        event->ignore();
}

void SchView::mouseReleaseEvent(QMouseEvent *event)
{
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

void SchView::updateMousePos()
{
    QPoint viewPos = mapFromGlobal(QCursor::pos());
    QPointF scenePos = mapToScene(viewPos);
    if (m_snapToGridEnabled &&
            scene() && scene()->grid()) {
        QPointF snapPos = scene()->grid()->snap(pixelSize(), scenePos);
        QPoint newPos = mapFromScene(snapPos);
        if (m_mousePosition != newPos) {
            m_mousePosition = newPos;
            m_mousePositionChanged = true;
            scene()->update(); // Force redraw of foreground
        }
        else {
            m_mousePositionChanged = false;
        }
    }
    else {
        m_mousePosition = viewPos;
        m_mousePositionChanged = true;
    }
}

QMouseEvent SchView::snapMouseEvent(QMouseEvent *event)
{
    return QMouseEvent(event->type(),
                       m_mousePosition,
                       event->button(),
                       event->buttons(),
                       event->modifiers());
}

QSizeF SchView::pixelSize() const
{
    return QSizeF(transform().m11(), transform().m22());
}
