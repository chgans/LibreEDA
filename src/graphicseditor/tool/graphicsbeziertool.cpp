#include "tool/graphicsbeziertool.h"
#include "item/graphicsbezieritem.h"

#include "graphicshandle.h"
#include "graphicsview.h"
#include "graphicsscene.h"

#include <QMouseEvent>

#include <QAction>
#include <QIcon>

#include <QLoggingCategory>
Q_DECLARE_LOGGING_CATEGORY(GraphicsBezierToolLog)
Q_LOGGING_CATEGORY(GraphicsBezierToolLog, "graphics.bezier.tool")
#define DEBUG() qCDebug(GraphicsBezierToolLog)
#define WARNING() qCDebug(GraphicsBezierToolLog)

#define WARN_STATE WARNING() << "Inconsistent state!"


GraphicsBezierTool::GraphicsBezierTool(QObject *parent):
    GraphicsTool(parent),
    m_state(NotStarted),
    m_item(nullptr),
    m_insertHandleOnMouseMove(false)
{

}

QPointF GraphicsBezierTool::mapToScene(const QPoint &pos)
{
    if (view() == nullptr)
        return QPointF();
    return view()->mapToScene(pos);
}

QPointF GraphicsBezierTool::mapToItem(const QPoint &pos)
{
    if (m_item == nullptr)
        return QPointF();
    return m_item->mapFromScene(mapToScene(pos));
}

QPointF GraphicsBezierTool::mapFromScene(const QPointF &pos)
{
    if (view() == nullptr)
        return QPointF();
    return view()->mapFromScene(pos);
}

QPointF GraphicsBezierTool::mapFromItem(const QPointF &pos)
{
    if (m_item == nullptr)
        return QPointF();
    return m_item->mapToScene(mapFromScene(pos));
}

void GraphicsBezierTool::setState(GraphicsBezierTool::State state)
{
    DEBUG() << "State" << m_state << "=>" << state;
    m_state = state;
}

void GraphicsBezierTool::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton) {
        return;
    }

    switch (m_state) {
    case NotStarted:
        scene()->clearSelection();
        m_nodePos = event->pos();
        setState(FirstPoint);
        break;
    case MidPoints:
        break;
    default:
        WARN_STATE;
        break;
    }

    event->accept();
}

void GraphicsBezierTool::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() != 0) {
        return; // Do nothing if dragging
    }

    QPointF scenePos;
    QPointF itemPos;
    switch (m_state) {
    case NotStarted:
        // Nothing to do
        break;
    case FirstPoint:
        if (m_insertHandleOnMouseMove) {
            // Insert the point at the mouse release position
            scenePos = mapToScene(m_nodePos);
            itemPos = mapToItem(m_nodePos);
            DEBUG() << "MME, adding first point" << scenePos << itemPos;
            m_index = m_item->addPoint(itemPos);
            setState(MidPoints);
            // Don't clear m_insertHandleOnMouseMove, so that the second point
            // will be inserted when the mouse will move
        }
        m_nodePos = event->pos();
        break;
    case MidPoints:
        m_nodePos = event->pos();
        scenePos = mapToScene(m_nodePos);
        itemPos = mapToItem(m_nodePos);
        if (m_insertHandleOnMouseMove) {
            DEBUG() << "MME, adding mid point" << scenePos << itemPos;
            m_index = m_item->addPoint(itemPos);
            m_insertHandleOnMouseMove = false;
        }
        else {
            m_item->movePoint(m_index, itemPos);
        }
        break;
    default:
        WARN_STATE;
        break;
    }

    event->accept();
}

void GraphicsBezierTool::mouseReleaseEvent(QMouseEvent *event)
{
    // FIXME: track which button was pressed!
    //if (!event->buttons().testFlag(Qt::LeftButton)) {
    //    return;
    //}

    QPointF scenePos = mapToScene(m_nodePos);
    QPointF itemPos = mapToItem(m_nodePos);
    switch (m_state) {
    case NotStarted:
        DEBUG() << "MRE, NS";
        break;
    case FirstPoint:
        scenePos = mapToScene(m_nodePos);
        DEBUG() << "MRE, Adding item at " << scenePos;
        m_item = new GraphicsBezierItem();
        m_item->setPos(scenePos);
        m_item->setPen(QPen(QBrush(Qt::darkCyan), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        m_item->setFlags(QGraphicsItem::ItemIsSelectable|QGraphicsItem::ItemIsMovable);
        scene()->addItem(m_item);
        m_item->setSelected(true); // For debugging
        itemPos = mapToItem(m_nodePos);
        DEBUG() << "MRE, scheduling add first point" << scenePos << itemPos;
        m_insertHandleOnMouseMove = true;
        break;
    case MidPoints:
        scenePos = mapToScene(m_nodePos);
        itemPos = mapToItem(m_nodePos);
        DEBUG() << "MRE, scheduling add mid point" << scenePos << itemPos;
        m_insertHandleOnMouseMove = true;
        break;
    case LastPoint:
        m_item = nullptr;
        setState(NotStarted);
        break;
    default:
        WARN_STATE;
        break;
    }

    event->accept();
}


void GraphicsBezierTool::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (!event->buttons().testFlag(Qt::LeftButton)) {
        return;
    }

    switch (m_state) {
    case FirstPoint:
        DEBUG() << "MDCE, FP";
        scene()->removeItem(m_item);
        delete m_item;
        m_item = nullptr;
        setState(NotStarted);
        break;
    case MidPoints:
        DEBUG() << "MDCE, MP";
        setState(LastPoint);
        break;
    default:
        WARN_STATE;
        break;
    }

    event->accept();
}

void GraphicsBezierTool::keyPressEvent(QKeyEvent *event)
{
}

void GraphicsBezierTool::keyReleaseEvent(QKeyEvent *event)
{
}

QDialog *GraphicsBezierTool::optionDialog()
{
    return nullptr;
}

QString GraphicsBezierTool::toolGroup() const
{
    return "interactive-tools";
}

QAction *GraphicsBezierTool::action() const
{
    return new QAction(QIcon(":/icons/graphicsbeziertool.svg"),
                                  "Place a bezier curve", nullptr);
}

void GraphicsBezierTool::activate()
{
    if (m_item != nullptr) {
        delete m_item;
    }
    m_state = NotStarted;
}

void GraphicsBezierTool::desactivate()
{
}

