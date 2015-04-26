#ifndef GRAPHICSLINEITEM_H
#define GRAPHICSLINEITEM_H

#include "schitem.h"

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
    GraphicsLineItem(SchItem *parent = 0);

    QLineF line() const;

public slots:
    void setLine(const QLineF &line);

signals:
    void lineChanged();

private:
    QLineF m_line;

    // SchItem interface
public:
    SchItem *clone();
    virtual bool fromJson(QString *errorString, const QJsonObject &jsonObject);
    virtual void toJson(QJsonObject &jsonObject) const;

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
