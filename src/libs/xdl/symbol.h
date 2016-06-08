#ifndef XDLSYMBOL_H
#define XDLSYMBOL_H

#include "xdl_global.h"

#include <QtGlobal>
#include <QPen>
#include <QBrush>
#include <QRectF>
#include <QFont>
#include <QIcon>
#include <QVariant>

namespace xdl {
namespace symbol {

    enum LineStyle
    {
        NoLine = 0,
        SolidLine = 1,
        DashLine = 2,
        DotLine = 3,
        DashDotLine = 4,
        DashDotDotLine = 5
    };

    enum LineWidth
    {
        ThinestLine = 0, // 0.13mm
        ThinerLine = 1, // 0.18mm
        ThinLine = 2, // 0.25mm
        SlightlyThinLine = 3, // 0.35mm
        MediumLine = 4, //0.50mm
        SlightlyThickLine = 5, // 0.70mm
        ThickLine = 6, // 1.0mm
        ThickerLine = 7, // 1.40mm
        ThickestLine = 8 // 2.00mm
    };

    enum Color
    {
        EmphasisedContent = 0,
        PrimaryContent = 1,
        SecondaryContent = 2,
        BackgroundHighLight = 3,
        Background = 4,
        Yellow = 5,
        Orange = 6,
        Red = 7,
        Magenta = 8,
        Violet = 9,
        Blue = 10,
        Cyan = 11,
        Green = 12
    };

class XDL_EXPORT Item
{
public:
    enum Type
    {
        Rectangle = 0,
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

    enum PropertyId
    {
        PositionProperty = 0,
        OpacityProperty,
        RotationProperty,
        LockedProperty,
        VisibilityProperty,
        XMirroredProperty,
        YMirroredProperty,

        LineStyleProperty,
        LineWidthProperty,
        LineColorProperty,
        FillColorProperty,

        RadiusProperty,
        XRadiusProperty,
        YRadiusProperty,
        StartAngleProperty,
        SpanAngleProperty,
        WidthProperty,
        HeightProperty,

        VerticesProperty,

        TextProperty,
        TextColorProperty,
        FontFamilyProperty,
        FontSizeProperty,
    };

    Item();
    virtual ~Item();

    virtual Type type() const = 0;
    virtual Item *clone() const = 0;

    virtual QString friendlyTypeName() const = 0;
    virtual QIcon icon() const = 0;

    virtual QVariant property(int id);
    virtual void setProperty(int id, const QVariant &value);

    void setPosition(const QPointF &pos);
    QPointF position() const;

    void setOpacity(qreal opacity);
    qreal opacity() const;

    void setRotation(qreal rotation);
    qreal rotation() const;

    void setLocked(bool locked);
    bool isLocked() const;

    void setVisible(bool visible);
    bool isVisible() const;

    void setXMirrored(bool mirrored);
    bool isXMirrored() const;

    void setYMirrored(bool mirrored);
    bool isYMirrored() const;

    void setLineStyle(LineStyle style);
    LineStyle lineStyle() const;

    void setLineWidth(LineWidth width);
    LineWidth lineWidth() const;

    void setLineColor(const QString &color);
    QString lineColor() const;

    void setFillColor(const QString &color);
    QString fillColor() const;

private:
    LineStyle m_lineStyle;
    LineWidth m_lineWidth;
    QString m_lineColor;
    QString m_fillColor;
    qreal m_opacity;
    QPointF m_position;
    qreal m_rotation;
    bool m_locked;
    bool m_visible;
    bool m_xMirrored;
    bool m_yMirrored;
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

    qreal width() const;
    void setWidth(qreal width);
    qreal height() const;
    void setHeight(qreal height);

private:
    qreal m_width;
    qreal m_height;

    // Item interface
public:
    Type type() const override;
    Item *clone() const override;
    QString friendlyTypeName() const override;
    QIcon icon() const override;
    QVariant property(int id) override;
    void setProperty(int id, const QVariant &value) override;
};

class XDL_EXPORT CircleItem: public Item
{
public:
    CircleItem();
    ~CircleItem();

    qreal radius() const;
    void setRadius(qreal radius);

private:
    qreal m_radius;

    // Item interface
public:
    Type type() const override;
    Item *clone() const override;
    virtual QString friendlyTypeName() const override;
    virtual QIcon icon() const override;
    QVariant property(int id) override;
    void setProperty(int id, const QVariant &value) override;

};

class XDL_EXPORT CircularArcItem: public Item
{
public:
    CircularArcItem();
    ~CircularArcItem();

    qreal radius() const;
    void setRadius(qreal radius);
    qreal startAngle() const;
    void setStartAngle(qreal angle);
    qreal spanAngle() const;
    void setSpanAngle(qreal angle);

private:
    qreal m_radius;
    qreal m_startAngle;
    qreal m_spanAngle;

    // Item interface
public:
    Type type() const override;
    Item *clone() const override;
    virtual QString friendlyTypeName() const override;
    virtual QIcon icon() const override;
    QVariant property(int id) override;
    void setProperty(int id, const QVariant &value) override;
};

class XDL_EXPORT EllipseItem: public Item
{
public:
    EllipseItem();
    ~EllipseItem();

    qreal xRadius() const;
    void setXRadius(qreal radius);
    qreal yRadius() const;
    void setYRadius(qreal radius);

private:
    qreal m_xRadius;
    qreal m_yRadius;

    // Item interface
public:
    Type type() const override;
    Item *clone() const override;
    virtual QString friendlyTypeName() const override;
    virtual QIcon icon() const override;
    QVariant property(int id) override;
    void setProperty(int id, const QVariant &value) override;
};

class XDL_EXPORT EllipticalArcItem: public Item
{
public:
    EllipticalArcItem();
    ~EllipticalArcItem();

    qreal xRadius() const;
    void setXRadius(qreal radius);
    qreal yRadius() const;
    void setYRadius(qreal radius);
    qreal startAngle() const;
    void setStartAngle(qreal angle);
    qreal spanAngle() const;
    void setSpanAngle(qreal angle);

private:
    qreal m_xRadius;
    qreal m_yRadius;
    qreal m_startAngle;
    qreal m_spanAngle;

    // Item interface
public:
    Type type() const override;
    Item *clone() const override;
    virtual QString friendlyTypeName() const override;
    virtual QIcon icon() const override;
    QVariant property(int id) override;
    void setProperty(int id, const QVariant &value) override;
};

class XDL_EXPORT PolylineItem: public Item
{
public:
    PolylineItem();
    ~PolylineItem();

    QList<QPointF> vertices() const;
    void setVertices(const QList<QPointF> &vertices);

private:
    QList<QPointF> m_vertices;

    // Item interface
public:
    Type type() const override;
    Item *clone() const override;
    virtual QString friendlyTypeName() const override;
    virtual QIcon icon() const override;
    QVariant property(int id) override;
    void setProperty(int id, const QVariant &value) override;
};

class XDL_EXPORT PolygonItem: public Item
{
public:
    PolygonItem();
    ~PolygonItem();

    QList<QPointF> vertices() const;
    void setVertices(const QList<QPointF> &vertices);

private:
    QList<QPointF> m_vertices;

    // Item interface
public:
    Type type() const override;
    Item *clone() const override;
    virtual QString friendlyTypeName() const override;
    virtual QIcon icon() const override;
    QVariant property(int id) override;
    void setProperty(int id, const QVariant &value) override;
};

// TBD: Text color
class XDL_EXPORT LabelItem: public Item
{
public:
    LabelItem();
    ~LabelItem();

    QString text() const;
    void setText(const QString &text);

    QString textColor() const;
    void setTextColor(const QString &color);

    QString fontFamily() const;
    void setFontFamily(const QString &family);

    int fontSize() const;
    void setFontSize(int size);

    // Item interface
public:
    Type type() const override;
    Item *clone() const override;
    virtual QString friendlyTypeName() const override;
    virtual QIcon icon() const override;
    QVariant property(int id) override;
    void setProperty(int id, const QVariant &value) override;

    QString m_text;
    QString m_textColor;
    QString m_fontFamily;
    int m_fontSize;
};

// TODO: pin length and deignator/label as QString vs LabelItem
class XDL_EXPORT PinItem: public Item
{
public:
    PinItem();
    ~PinItem();


    // Item interface
public:
    Type type() const override;
    Item *clone() const override;
    virtual QString friendlyTypeName() const override;
    virtual QIcon icon() const override;
    QVariant property(int id) override;
    void setProperty(int id, const QVariant &value) override;

    LabelItem *designator;
    LabelItem *label;
};

class XDL_EXPORT ItemGroup: public Item
{
public:
    ItemGroup();
    ~ItemGroup();


    // Item interface
public:
    Type type() const override;
    Item *clone() const override;
    virtual QString friendlyTypeName() const override;
    virtual QIcon icon() const override;
    QVariant property(int id) override;
    void setProperty(int id, const QVariant &value) override;

    QList<Item *> children;
};

// Text (frame)

}
}
#endif // XDLSYMBOL_H
