#include "graphicsview.h"
#include "graphicsscene.h"
#include "graphicsobject.h"
#include "abstractgraphicsinteractivetool.h"
#include "graphicsgrid.h"

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

GraphicsView::GraphicsView(QWidget *parent):
    QGraphicsView(parent),
    m_tool(nullptr),
    m_objectUnderMouse(nullptr),
    m_handleUnderMouse(nullptr),
    m_mousePositionChanged(true),
    m_snapToGridEnabled(true)
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

GraphicsView::~GraphicsView()
{

}

GraphicsScene *GraphicsView::scene()
{
    return static_cast<GraphicsScene *>(QGraphicsView::scene());
}

void GraphicsView::setScene(GraphicsScene *scene)
{
    QGraphicsView::setScene(scene);
}

AbstractGraphicsInteractiveTool *GraphicsView::tool()
{
    return m_tool;
}

void GraphicsView::setTool(AbstractGraphicsInteractiveTool *tool)
{
    if (m_tool)
        m_tool->desactivate(m_tool->action()); // FIXME: pass "this" instead, a non active tool doesnt need a view
    m_tool = tool;
    if (m_tool) {
        m_tool->setView(this);
        m_tool->activate(tool->action());
    }
}

GraphicsObject *GraphicsView::objectAt(const QPoint &pos) const
{
    return dynamic_cast<GraphicsObject *>(QGraphicsView::itemAt(pos));
}

GraphicsHandle *GraphicsView::handleAt(const QPoint &pos) const
{
    return dynamic_cast<GraphicsHandle *>(QGraphicsView::itemAt(pos));
}

GraphicsHandle *GraphicsView::handleUnderMouse() const
{
    return handleAt(mousePosition());
}

GraphicsObject *GraphicsView::objectUnderMouse() const
{
    return objectAt(mousePosition());
}

QPoint GraphicsView::mousePosition() const
{
    return m_mousePosition;
}

void GraphicsView::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;

    scale(scaleFactor, scaleFactor);
    updateMousePos();
}

void GraphicsView::enableSnapToGrid(bool enabled)
{
    m_snapToGridEnabled = enabled;
}

bool GraphicsView::isSnapToGridEnabled() const
{
    return m_snapToGridEnabled;
}

void GraphicsView::drawBackground(QPainter *painter, const QRectF &rect)
{

    QLinearGradient gradient(rect.topLeft(), rect.bottomLeft());
    gradient.setColorAt(0, Qt::darkBlue);
    gradient.setColorAt(1, Qt::lightGray);
    painter->fillRect(rect, QBrush(gradient));

    QGraphicsView::drawBackground(painter, rect);

    if (scene() && scene()->grid()) {
        scene()->grid()->draw(pixelSize(), painter, rect);
    }
}

void GraphicsView::drawForeground(QPainter *painter, const QRectF &rect)
{
    //QGraphicsView::drawForeground(painter, rect);

    QPointF p = mapToScene(mousePosition());

    if (p.isNull())
        return;

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
void GraphicsView::wheelEvent(QWheelEvent *event)
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

void GraphicsView::mousePressEvent(QMouseEvent *event)
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

void GraphicsView::mouseMoveEvent(QMouseEvent *event)
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

void GraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    updateMousePos();

    if (m_tool != nullptr) {
        QMouseEvent ev = snapMouseEvent(event);
        m_tool->mouseReleaseEvent(&ev);
    }
    else
        event->ignore();
}

void GraphicsView::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (m_tool != nullptr)
        m_tool->mouseDoubleClickEvent(event);
    else
        event->ignore();
}

// TODO: manage esc and tab here or tool or tool manager?
//  => better in AbstractGraphicsTool... no
//  => a Tool manager? Someone as to manage the "default" tool logic
//  => Pass the events to tool manager, and then depending of the accepted
//     state of the event and the kind of event, pass it to the scene
//  => As well, we should certainly use QAction as well
void GraphicsView::keyPressEvent(QKeyEvent *event)
{
    if (m_tool != nullptr)
        m_tool->keyPressEvent(event);
    else
        event->ignore();
}

void GraphicsView::keyReleaseEvent(QKeyEvent *event)
{
    if (m_tool != nullptr)
        m_tool->keyReleaseEvent(event);
    else
        event->ignore();
}

void GraphicsView::updateMousePos()
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

QMouseEvent GraphicsView::snapMouseEvent(QMouseEvent *event)
{
    return QMouseEvent(event->type(),
                       m_mousePosition,
                       event->button(),
                       event->buttons(),
                       event->modifiers());
}

QSizeF GraphicsView::pixelSize() const
{
    return QSizeF(transform().m11(), transform().m22());
}
