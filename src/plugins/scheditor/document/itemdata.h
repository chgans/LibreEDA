#ifndef ITEMDATA_H
#define ITEMDATA_H

#include <QPen>
#include <QBrush>
#include <QRectF>

struct ItemData
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

struct RectangleData: public ItemData
{
    QPointF topLeft;
    QPointF bottomRight;
};

struct CircleData: public ItemData
{
    QPointF center;
    qreal radius;
};

struct CircularArcData: public ItemData
{
    QPointF center;
    qreal radius;
    qreal startAngle;
    qreal spanAngle;
};

struct EllipseData: public ItemData
{
    QPointF center;
    qreal xRadius;
    qreal yRadius;
};

struct EllipticalArcData: public ItemData
{
    QPointF center;
    qreal xRadius;
    qreal yRadius;
    qreal startAngle;
    qreal spanAngle;
};

struct PolyLineData: public ItemData
{
    QList<QPointF> vertices;
};

struct PolygonData: public ItemData
{
    QList<QPointF> vertices;
};

struct LabelData: public ItemData
{
    QString text;
};

struct PinData: public ItemData
{
    LabelData designator;
    LabelData label;
};

struct ItemGroupData: public ItemData
{
    QList<int> childrenId;
};

// Text (frame)

#endif // ITEMDATA_H
