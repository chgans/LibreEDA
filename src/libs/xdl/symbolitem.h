#ifndef XDLSYMBOLITEM_H
#define XDLSYMBOLITEM_H

#include "xdl_global.h"

#include <QtGlobal>
#include <QPen>
#include <QBrush>
#include <QRectF>

namespace xdl { namespace symbol {

struct XDL_EXPORT Item
{
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

struct XDL_EXPORT RectangleItem: public Item
{
    QPointF topLeft;
    QPointF bottomRight;
};

struct XDL_EXPORT CircleItem: public Item
{
    QPointF center;
    qreal radius = 0.0f;
};

struct XDL_EXPORT CircularArcItem: public Item
{
    QPointF center;
    qreal radius = 0.0f;
    qreal startAngle = 0.0f;
    qreal spanAngle = 360.0f;
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
    qreal xRadius = 0.0f;
    qreal yRadius = 0.0f;
    qreal startAngle = 0.0f;
    qreal spanAngle = 360.0f;
};

struct XDL_EXPORT PolylineItem: public Item
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

}}
#endif // XDLSYMBOLITEM_H
