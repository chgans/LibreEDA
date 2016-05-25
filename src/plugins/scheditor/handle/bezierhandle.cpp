#include "handle/bezierhandle.h"
#include "handle/regularhandle.h"
#include "item/item.h"

#include <QLoggingCategory>
Q_DECLARE_LOGGING_CATEGORY(GraphicsBezierHandleLog)
Q_LOGGING_CATEGORY(GraphicsBezierHandleLog, "graphics.bezier.handle")
#define DEBUG() qCDebug(GraphicsBezierHandleLog)
#define WARNING() qCDebug(GraphicsBezierHandleLog)

using namespace SymbolEditor;

BezierHandle::BezierHandle(Item *parent):
    Handle(parent),
    IItemObserver(),
    m_first(false),
    m_last(false),
    m_closing(false),
    m_nodeHandle(new RegularHandle()),
    m_control1Handle(new RegularHandle()),
    m_control2Handle(new RegularHandle()),
    m_behaviour(NormalHandleBehaviour)
{
    m_typeToHandle[NodeHandle] = &m_nodeHandle;
    m_typeToHandle[Control1Handle] = &m_control1Handle;
    m_typeToHandle[Control2Handle] = &m_control2Handle;

    m_nodeHandle->setParentItem(this);
    m_nodeHandle->setHandleRole(MoveHandleRole);
    m_nodeHandle->setHandleShape(CircularHandleShape);
    m_nodeHandle->setVisible(false);
    m_nodeHandle->setHandleId(-1);
    m_nodeHandle->setParentGraphicsObject(nullptr);

    m_control1Handle->setParentItem(this);
    m_control1Handle->setHandleRole(MoveHandleRole);
    m_control1Handle->setHandleShape(DiamondedHandleShape);
    m_control1Handle->setVisible(false);
    m_control1Handle->setHandleId(-1);
    m_control1Handle->setParentGraphicsObject(nullptr);

    m_control2Handle->setParentItem(this);
    m_control2Handle->setHandleRole(MoveHandleRole);
    m_control2Handle->setHandleShape(DiamondedHandleShape);
    m_control2Handle->setVisible(false);
    m_control2Handle->setHandleId(-1);
    m_control2Handle->setParentGraphicsObject(nullptr);

    blockItemNotification();
    addObservedItem(m_nodeHandle);
    addObservedItem(m_control1Handle);
    addObservedItem(m_control2Handle);
    updateEnabledHandles();
    unblockItemNotification();

    QGraphicsItem::setPos(QPointF(0, 0));
    DEBUG() << "Our pos" << mapToParent(QGraphicsItem::pos());
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    setSelected(false);
    setVisible(false);
}

BezierHandle::~BezierHandle()
{
    // TODO: use map
    if (m_nodeHandle)
    {
        removeObservedItem(m_nodeHandle);
        delete m_nodeHandle;
    }
    if (m_control1Handle)
    {
        removeObservedItem(m_control1Handle);
        delete m_control1Handle;
    }
    if (m_control2Handle)
    {
        removeObservedItem(m_control2Handle);
        delete m_control2Handle;
    }
    // removeItemObserver(); // ?
}

QPointF BezierHandle::pos(BezierHandle::HandleType type) const
{
    Q_ASSERT(m_handles.testFlag(type));

    // TODO: use map
    Handle *handle;
    switch (type)
    {
        case NodeHandle:
            handle = m_nodeHandle;
            break;
        case Control1Handle:
            handle = m_control1Handle;
            break;
        case Control2Handle:
            handle = m_control2Handle;
            break;
        default:
            Q_ASSERT(false);
    }

    QPointF handlePos = handle->pos();
    QPointF parentPos = mapToParent(handlePos);
    return parentPos;
}

// Pos is in parent coordinate
void BezierHandle::setPos(BezierHandle::HandleType type, const QPointF &pos)
{
    Q_ASSERT(m_handles.testFlag(type));

    blockItemNotification();

    // TODO: use map
    Handle *handle;
    switch (type)
    {
        case NodeHandle:
            handle = m_nodeHandle;
            break;
        case Control1Handle:
            handle = m_control1Handle;
            break;
        case Control2Handle:
            handle = m_control2Handle;
            break;
        default:
            Q_ASSERT(false);
    }

    QPointF parentPos = pos;
    QPointF handlePos = mapFromParent(parentPos); // Same if we're at (0, 0)
    DEBUG() << "Set point " << handleId() << typeToString(type)
            << " pos. parent=" << parentPos
            << ", handle=" << handlePos;

    handle->setPos(handlePos);

    unblockItemNotification();
}

void BezierHandle::setBehaviour(GraphicsHandleBehaviour behaviour)
{
    if (m_behaviour == behaviour)
    {
        return;
    }

    m_behaviour = behaviour;
    updateEnabledHandles();
}

GraphicsHandleBehaviour BezierHandle::behaviour() const
{
    return m_behaviour;
}

BezierHandle::HandleTypes BezierHandle::handlesEnabled() const
{
    return m_handles;
}

void BezierHandle::enableHandle(BezierHandle::HandleType type, bool set)
{
    Handle **handle = m_typeToHandle[type];
    Q_ASSERT(handle != nullptr);

    if (set)
    {
        m_handles |= type;
        (*handle)->setVisible(true);
    }
    else
    {
        m_handles &= ~type;
        (*handle)->setVisible(false);
    }
}

bool BezierHandle::isFirst() const
{
    return m_first;
}

void BezierHandle::setFirst(bool first)
{
    if (m_first == first)
    {
        return;
    }

    m_first = first;
    updateEnabledHandles();
}

bool BezierHandle::isLast() const
{
    return m_last;
}

void BezierHandle::setLast(bool last)
{
    if (m_last == last)
    {
        return;
    }

    m_last = last;
    updateEnabledHandles();
}

bool BezierHandle::isClosingPath() const
{
    return m_closing;
}

void BezierHandle::setClosingPath(bool closing)
{
    if (m_closing == closing)
    {
        return;
    }
    m_closing = closing;
    updateEnabledHandles();
}

QString BezierHandle::typeToString(BezierHandle::HandleType type)
{
    switch (type)
    {
        case NodeHandle:
            return QString("Node");
        case Control1Handle:
            return QString("Ctl1");
        case Control2Handle:
            return QString("Ctl2");
        default:
            return QString("???");
    }
}

bool BezierHandle::handleEnabled(BezierHandle::HandleType type)
{
    return m_handles.testFlag(type);
}

void BezierHandle::updateEnabledHandles()
{
    enableHandle(NodeHandle, true);
    enableHandle(Control1Handle, (isFirst() && isClosingPath()) || !isFirst());
    enableHandle(Control2Handle, (isLast() && isClosingPath()) || !isLast());
}

void BezierHandle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                                 QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->save();

    painter->setPen(QPen(QBrush(Qt::green), 0));

    if (m_handles.testFlag(Control2Handle))
    {
        QLineF line(nodePos(), control2Pos());
        painter->drawLine(line);
    }

    if (m_handles.testFlag(Control1Handle))
    {
        QLineF line(nodePos(), control1Pos());
        painter->drawLine(line);
    }

    painter->restore();
}

void BezierHandle::itemNotification(IObservableItem *item)
{
    Handle *handle = dynamic_cast<Handle *>(item);
    Q_ASSERT(handle != nullptr);
    if (handle == m_nodeHandle)
    {
        Q_ASSERT(m_handles.testFlag(NodeHandle));
    }
    else if (item == m_control1Handle)
    {
        Q_ASSERT(m_handles.testFlag(Control1Handle));
    }
    else if (item == m_control2Handle)
    {
        Q_ASSERT(m_handles.testFlag(Control2Handle));
    }
    else
    {
        Q_ASSERT(false);
    }
    blockItemNotification();
    notifyObservers();
    unblockItemNotification();

    prepareGeometryChange();
    update();
}

QRectF BezierHandle::boundingRect() const
{
    return childrenBoundingRect();
}

QPainterPath BezierHandle::shape() const
{
    return QPainterPath();
}

void BezierHandle::setHandleId(int id)
{
    Handle::setHandleId(id);
    // FIXME: needed because of view->handleUnderMouse()->{parent(),id()}
    m_nodeHandle->setHandleId(id);
    m_control1Handle->setHandleId(id);
    m_control2Handle->setHandleId(id);
}

void BezierHandle::setParentGraphicsObject(Item *parent)
{
    Handle::setParentGraphicsObject(parent);
    // FIXME: needed because of view->handleUnderMouse()->{parent(),id()}
    m_nodeHandle->setParentGraphicsObject(parent);
    m_control1Handle->setParentGraphicsObject(parent);
    m_control2Handle->setParentGraphicsObject(parent);
}
