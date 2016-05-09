#ifndef XDLSYMBOLITEM_H
#define XDLSYMBOLITEM_H

#include "xdl_global.h"

#include <QPen>
#include <QBrush>
#include <QRectF>

namespace xdl
{
namespace symbol
{
  
struct XDL_EXPORT Item
{
    uint64_t itemId;
    QPen pen;
    QBrush brush;
    qreal opacity;
    QPointF position;
    qreal rotation;
    qreal zValue;
    bool locked;
    bool visible;
    bool xMirrored;
    bool yMirrored;
};

struct XDL_EXPORT RectangleItem: public Item
{
    QPointF topLeft;
    QPointF bottomRight;
};

struct XDL_EXPORT CircleItem: public Item
{
    QPointF center;
    qreal radius;
};

struct XDL_EXPORT CircularArcItem: public Item
{
    QPointF center;
    qreal radius;
    qreal startAngle;
    qreal spanAngle;
};

struct XDL_EXPORT EllipseItem: public Item
{
    QPointF center;
    qreal xRadius;
    qreal yRadius;
};

struct XDL_EXPORT EllipticalArcItem: public Item
{
    QPointF center;
    qreal xRadius;
    qreal yRadius;
    qreal startAngle;
    qreal spanAngle;
};

struct XDL_EXPORT PolyLineItem: public Item
{
    QList<QPointF> vertices;
};

struct XDL_EXPORT PolygonItem: public Item
{
    QList<QPointF> vertices;
};

struct XDL_EXPORT LabelItem: public Item
{
    QString text;
};

struct XDL_EXPORT PinItem: public Item
{
    LabelItem designator;
    LabelItem label;
};

struct XDL_EXPORT ItemGroup: public Item
{
    QList<int> childrenId;
};

// Text (frame)

}
}
#endif // XDLSYMBOLITEM_H
