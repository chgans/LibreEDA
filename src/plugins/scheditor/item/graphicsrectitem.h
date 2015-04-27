#ifndef GRAPHICSRECTITEM_H
#define GRAPHICSRECTITEM_H

#include "schitem.h"

#include <QVector>
#include <QMap>

#include <QPen>
#include <QBrush>

// TODO: AbstractGraphicsShape
// TODO: add properties
class GraphicsRectItem : public SchItem
{
    Q_OBJECT

public:
    enum HandleId {
        TopLeft = 0,
        Top,
        TopRight,
        Right,
        BottomRight,
        Bottom,
        BottomLeft,
        Left,
        NbHandles
    };

    GraphicsRectItem(SchItem *parent = 0);
    ~GraphicsRectItem();

    QRectF rect() const;
    void setRect(const QRectF &rect);

private:
    static const QString J_POINTS;
    QRectF m_rect;
    void updateHandlesSilently();

    // ScheItem interface
public:
    virtual SchItem *clone();
    void itemNotification(IGraphicsObservableItem *item);
    virtual bool fromJson(QString *errorString, const QJsonObject &jsonObject);
    virtual void toJson(QJsonObject &jsonObject) const;

    // QGraphicsItem interface
public:
    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    // QGraphicsItem interface
protected:
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);
};

#endif // GRAPHICSRECTITEM_H
