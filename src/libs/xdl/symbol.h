#ifndef XDLSYMBOL_H
#define XDLSYMBOL_H

#include "xdl_global.h"

#include <QtGlobal>
#include <QPen>
#include <QBrush>
#include <QRectF>
#include <QFont>

namespace xdl {
namespace symbol {

class XDL_EXPORT Item
{
public:
    enum Type
    {
        Rectangle,
        Circle,
        CircularArc,
        Ellipse,
        EllipticalArc,
        Polyline,
        Polygon,
        Label,
        Pin,
        Group
    };

    Item();
    virtual ~Item();

    virtual Type type() const = 0;

    QPen pen;
    QBrush brush;
    qreal opacity = 1.0f;
    QPointF position;
    qreal rotation = 0.0f;
    qreal zValue = 0.0;
    bool locked = false;
    bool visible = true;
    bool xMirrored = false;
    bool yMirrored = false;
};

class XDL_EXPORT Symbol
{
public:
    Symbol();
    ~Symbol();

    QString name;
    QString description;
    QString designatorPrefix;
    QList<Item *> drawingItems;
};

class XDL_EXPORT RectangleItem: public Item
{
public:
    RectangleItem();
    ~RectangleItem();

    Type type() const
    {
        return Rectangle;
    }

    QPointF topLeft;
    QPointF bottomRight;
};

class XDL_EXPORT CircleItem: public Item
{
public:
    CircleItem();
    ~CircleItem();

    Type type() const
    {
        return Circle;
    }

    QPointF center;
    qreal radius = 0.0f;
};

class XDL_EXPORT CircularArcItem: public Item
{
public:
    CircularArcItem();
    ~CircularArcItem();

    Type type() const
    {
        return CircularArc;
    }

    QPointF center;
    qreal radius = 0.0f;
    qreal startAngle = 0.0f;
    qreal spanAngle = 360.0f;
};

class XDL_EXPORT EllipseItem: public Item
{
public:
    EllipseItem();
    ~EllipseItem();

    Type type() const
    {
        return Ellipse;
    }

    QPointF center;
    qreal xRadius;
    qreal yRadius;
};

class XDL_EXPORT EllipticalArcItem: public Item
{
public:
    EllipticalArcItem();
    ~EllipticalArcItem();

    Type type() const
    {
        return EllipticalArc;
    }

    QPointF center;
    qreal xRadius = 0.0f;
    qreal yRadius = 0.0f;
    qreal startAngle = 0.0f;
    qreal spanAngle = 360.0f;
};

class XDL_EXPORT PolylineItem: public Item
{
public:
    PolylineItem();
    ~PolylineItem();

    Type type() const
    {
        return Polyline;
    }

    QList<QPointF> vertices;
};

class XDL_EXPORT PolygonItem: public Item
{
public:
    PolygonItem();
    ~PolygonItem();

    Type type() const
    {
        return Polygon;
    }

    QList<QPointF> vertices;
};

class XDL_EXPORT LabelItem: public Item
{
public:
    LabelItem();
    ~LabelItem();

    Type type() const
    {
        return Label;
    }

    QString text;
    QFont font;
};

// TODO: pin length and deignator/label as QString vs LabelItem
class XDL_EXPORT PinItem: public Item
{
public:
    PinItem();
    ~PinItem();

    Type type() const
    {
        return Pin;
    }

    LabelItem *designator;
    LabelItem *label;
};

class XDL_EXPORT ItemGroup: public Item
{
public:
    ItemGroup();
    ~ItemGroup();

    Type type() const
    {
        return Group;
    }

    QList<Item *> children;
};

// Text (frame)

}
}
#endif // XDLSYMBOL_H
