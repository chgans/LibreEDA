#ifndef GRAPHICSBEZIERHANDLE_H
#define GRAPHICSBEZIERHANDLE_H

#include <QGraphicsItem>
#include "handle/abstractgraphicshandle.h"
#include "item/igraphicsitemobserver.h"
#include "item/igraphicsobservableitem.h"


class GraphicsBezierHandle:
    public AbstractGraphicsHandle,
    public IGraphicsItemObserver
{
public:
    explicit GraphicsBezierHandle(SchItem *parent = nullptr);
    ~GraphicsBezierHandle();

    enum HandleType
    {
        UndefinedHandle = 0,
        NodeHandle = 1,
        Control1Handle = 2,
        Control2Handle = 4
    };
    Q_DECLARE_FLAGS(HandleTypes, HandleType)

    QPointF nodePos() const
    {
        return pos(NodeHandle);
    }
    void setNodePos(const QPointF &pos)
    {
        setPos(NodeHandle, pos);
    }
    void setNodePos(qreal x, qreal y)
    {
        setNodePos(QPointF(x, y));
    }
    QPointF control1Pos() const
    {
        return pos(Control1Handle);
    }
    void setControl1Pos(const QPointF &pos)
    {
        setPos(Control1Handle, pos);
    }
    void setControl1Pos(qreal x, qreal y)
    {
        setControl1Pos(QPointF(x, y));
    }
    QPointF control2Pos() const
    {
        return pos(Control2Handle);
    }
    void setControl2Pos(const QPointF &pos)
    {
        setPos(Control2Handle, pos);
    }
    void setControl2Pos(qreal x, qreal y)
    {
        setControl2Pos(QPointF(x, y));
    }
    QPointF pos(HandleType type) const;
    void setPos(HandleType type, const QPointF &pos);
    void setPos(HandleType type, qreal x, qreal y)
    {
        setPos(type, QPointF(x, y));
    }

    void setBehaviour(GraphicsHandleBehaviour behaviour);
    GraphicsHandleBehaviour behaviour() const;


    HandleTypes handlesEnabled() const;
    bool handleEnabled(HandleType type);

protected:
    void updateEnabledHandles();
    //void enableHandles(HandleTypes types);
    void enableHandle(HandleType type, bool set = true);

public:
    bool isFirst() const;
    void setFirst(bool first);
    bool isLast() const;
    void setLast(bool last);
    bool isClosingPath() const;
    void setClosingPath(bool closing);

private:
    bool m_first;
    bool m_last;
    bool m_closing;
    HandleTypes m_handles;
    AbstractGraphicsHandle *m_nodeHandle;
    AbstractGraphicsHandle *m_control1Handle;
    AbstractGraphicsHandle *m_control2Handle;
    GraphicsHandleBehaviour m_behaviour;
    QMap<HandleType, AbstractGraphicsHandle **> m_typeToHandle;
    IGraphicsItemObserver *m_observer;
    QString typeToString(HandleType type);

    // IGraphicsHandleObserver interface
public:
    virtual void itemNotification(IGraphicsObservableItem *item);

    // QGraphicsItem interface
public:
    virtual QRectF boundingRect() const;
    QPainterPath shape() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    // AbstractGraphicsHandle interface
public:
    void setHandleId(int id);
    void setParentGraphicsObject(SchItem *parent);
};

Q_DECLARE_OPERATORS_FOR_FLAGS(GraphicsBezierHandle::HandleTypes)

#endif // GRAPHICSBEZIERHANDLE_H
