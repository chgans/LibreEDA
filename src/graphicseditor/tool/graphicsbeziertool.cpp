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
        qDebug() << "MPE, NS";
        scene()->clearSelection();
        m_nodePos = event->pos();
        setState(FirstPoint);
        break;
    case MidPoints:
        qDebug() << "MPE, MP";
        m_nodePos = event->pos();
        break;
    default:
        qWarning() << "Inconsistent state in MPE!";
        break;
    }

    event->accept();
}

void GraphicsBezierTool::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons().testFlag(Qt::LeftButton)) {
        return; // Do nothing if dragging
    }

    switch (m_state) {
    case NotStarted:
        // Nothing to do
        break;
    case FirstPoint:
        if (m_insertHandleOnMouseMove) { // TODO: check for minimum view distance, 10 pixels?
            // Insert the point at the mouse press position
            qDebug() << "MME, FP, A" << m_nodePos << mapToItem(m_nodePos);
            m_item->addPoint(mapToItem(m_nodePos));
            m_insertHandleOnMouseMove = false;
            // Insert the 2nd point at the mouse move position
            m_item->addPoint(mapToItem(event->pos()));
            setState(MidPoints);
        }
        m_nodePos = event->pos();
        break;
    case MidPoints: {
        if (m_insertHandleOnMouseMove) { // Same as above
            // Insert the point at the mouse release position
            qDebug() << "MME, MP, A" << m_nodePos << mapToItem(m_nodePos);
            m_item->addPoint(mapToItem(m_nodePos));
            m_insertHandleOnMouseMove = false;
        }
        m_nodePos = event->pos();
        int idx = m_item->pointCount() - 1;
        GraphicsPathPoint *point = m_item->pointAt(idx);
        point->setNodePos(mapToItem(m_nodePos));
        break;
    }
    default:
        qWarning() << "Inconsistent state in MVE!";
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

    switch (m_state) {
    case NotStarted:
        qDebug() << "MRE, NS";
        break;
    case FirstPoint:
        qDebug() << "MRE, FP, A" << mapToScene(event->pos());
        m_item = new GraphicsBezierItem();
        m_item->setPos(mapToScene(m_nodePos));
        m_item->setPen(QPen(QBrush(Qt::darkCyan), 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        m_item->setFlags(QGraphicsItem::ItemIsSelectable|QGraphicsItem::ItemIsMovable);
        scene()->addItem(m_item);
        m_item->setSelected(true); // For debugging
        m_insertHandleOnMouseMove = true;
        break;
    case MidPoints:
        qDebug() << "MRE, MP, A" << mapToItem(event->pos());
        m_insertHandleOnMouseMove = true;
        break;
    case LastPoint:
        m_item = nullptr;
        setState(NotStarted);
        break;
    default:
        qWarning() << "Inconsistent state in MRE!";
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
        qDebug() << "MDCE, FP";
        scene()->removeItem(m_item);
        delete m_item;
        m_item = nullptr;
        setState(NotStarted);
        break;
    case MidPoints:
        qDebug() << "MDCE, MP";
        // TODO: check the edge case when user just double click:
        // => only 2 points with almost same pos
        setState(LastPoint);
        break;
    default:
        qWarning() << "Inconsistent state MDCE!";
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
}

void GraphicsBezierTool::desactivate()
{
}

