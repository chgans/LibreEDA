#ifndef GRAPHICSLINEITEM_H
#define GRAPHICSLINEITEM_H

#include "item/schitem.h"

#include <QPen>
#include <QLineF>

class GraphicsLineItem : public SchItem
{
    Q_OBJECT

    Q_PROPERTY(QLineF line READ line WRITE setLine NOTIFY lineChanged)

    // GraphicsLineItem
public:
    enum HandleId {
        P1Handle = 0,
        P2Handle,
        NbHandles
    };
    explicit GraphicsLineItem(SchItem *parent = nullptr);

    QLineF line() const;

public slots:
    void setLine(const QLineF &line);

signals:
    void lineChanged();

private:
    static const QString J_POINTS;

    QLineF m_line;

    // SchItem interface
public:
    SchItem *clone();
    virtual bool fromJson(QString *errorString, const QJsonObject &jsonObject);
    virtual void toJson(QJsonObject &jsonObject) const;

    //virtual QList<QPointF> hotSpots() const;
    virtual QList<QPointF> endPoints() const;
    virtual QList<QPointF> midPoints() const;
    virtual QList<QPointF> centerPoints() const;
    virtual QList<QPointF> nearestPoints(QPointF pos) const;

    // IGraphicsItemObserver
public:
    void itemNotification(IGraphicsObservableItem *item);

    // QGraphicsItem interface
public:
    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    // QGraphicsItem interface
protected:
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);
};

#endif // GRAPHICSLINEITEM_H
