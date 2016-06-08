// Not copyrighted - public domain.
//
// This sample parser implementation was generated by CodeSynthesis XSD,
// an XML Schema to C++ data binding compiler. You may use it in your
// programs without any restrictions.
//

#ifndef LEDA_XDL_SYMBOL_PIMPL_HXX
#define LEDA_XDL_SYMBOL_PIMPL_HXX

#include "xdl/leda-xdl-symbol-pskel.hxx"

namespace xdl {
namespace symbol {

class Symbol_pimpl: public virtual Symbol_pskel
{
public:
    virtual void pre();
    virtual void name(const ::std::string &);
    virtual void label(const ::std::string &);
    virtual void drawing(const QList<Item *> &);
    virtual Symbol *post_Symbol();

private:
    QString m_name;
    QString m_label;
    QList<Item *> m_drawing;
};

class ItemList_pimpl: public virtual ItemList_pskel
{
public:
    virtual void pre();
    virtual void polyline(PolylineItem *);
    virtual void polygon(PolygonItem *);
    virtual void rectangle(RectangleItem *);
    virtual void circle(CircleItem *);
    virtual void circular_arc(CircularArcItem *);
    virtual void ellipse(EllipseItem *);
    virtual void elliptical_arc(EllipticalArcItem *);
    virtual void label(LabelItem *);
    virtual void pin(PinItem *);
    virtual void group(ItemGroup *);
    virtual QList<Item *> post_ItemList();

private:
    QList<Item *> m_items;
};

class Item_pimpl: public virtual Item_pskel
{
public:
    virtual void pre();
    virtual void pen(const QPen &);
    virtual void brush(const QBrush &);
    virtual void position(const QPointF &);
    virtual void z_value(double);
    virtual void rotation(const qreal &);
    virtual void opacity(const qreal &);
    virtual void locked(bool);
    virtual void x_mirrored(bool);
    virtual void y_mirrored(bool);
    virtual void visible(bool);
    virtual void post_Item();

protected:
    Item *m_item;

protected:
    QPen m_pen;
    QBrush m_brush;
    QPointF m_position;
    qreal m_zValue;
    qreal m_rotation;
    qreal m_opacity;
    bool m_locked;
    bool m_xMirrored;
    bool m_yMirrored;
    bool m_visible;
};

class Circle_pimpl: public virtual Circle_pskel,
    public Item_pimpl
{
public:
    virtual void pre();
    virtual void radius(const qreal &);
    virtual CircleItem *post_Circle();

private:
    qreal m_radius;
};

class CircularArc_pimpl: public virtual CircularArc_pskel,
    public Item_pimpl
{
public:
    virtual void pre();
    virtual void radius(const qreal &);
    virtual void start_angle(const qreal &);
    virtual void span_angle(const qreal &);
    virtual CircularArcItem *post_CircularArc();

private:
    qreal m_radius;
    qreal m_startAngle;
    qreal m_spanAngle;
};

class Ellipse_pimpl: public virtual Ellipse_pskel,
    public Item_pimpl
{
public:
    virtual void pre();
    virtual void x_radius(const qreal &);
    virtual void y_radius(const qreal &);
    virtual EllipseItem *post_Ellipse();

private:
    qreal m_xRadius;
    qreal m_yRadius;
};

class EllipticalArc_pimpl: public virtual EllipticalArc_pskel,
    public Item_pimpl
{
public:
    virtual void pre();
    virtual void x_radius(const qreal &);
    virtual void y_radius(const qreal &);
    virtual void start_angle(const qreal &);
    virtual void span_angle(const qreal &);
    virtual EllipticalArcItem *post_EllipticalArc();

private:
    qreal m_xRadius;
    qreal m_yRadius;
    qreal m_startAngle;
    qreal m_spanAngle;
};

class Rectangle_pimpl: public virtual Rectangle_pskel,
    public Item_pimpl
{
public:
    virtual void pre();
    virtual void width(const qreal &);
    virtual void height(const qreal &);
    virtual RectangleItem *post_Rectangle();

private:
    qreal m_width;
    qreal m_height;
};

class Polyline_pimpl: public virtual Polyline_pskel,
    public Item_pimpl
{
public:
    virtual void pre();
    virtual void vertices(const QList<QPointF> &);
    virtual PolylineItem *post_Polyline();

private:
    QList<QPointF> m_vertices;
};

class Polygon_pimpl: public virtual Polygon_pskel,
    public Item_pimpl
{
public:
    virtual void pre();
    virtual void vertices(const QList<QPointF> &);
    virtual PolygonItem *post_Polygon();

private:
    QList<QPointF> m_vertices;
};

class Pin_pimpl: public virtual Pin_pskel,
    public Item_pimpl
{
public:
    virtual void pre();
    virtual void designator(const ::std::string &);
    virtual void label(const ::std::string &);
    virtual PinItem *post_Pin();

private:
    QString m_designator;
    QString m_label;
};

class ItemGroup_pimpl: public virtual ItemGroup_pskel,
    public Item_pimpl
{
public:
    virtual void pre();
    virtual void children(const QList<Item *> &items);
    virtual ItemGroup *post_ItemGroup();

private:
    QList<Item *> m_children;
};



class Label_pimpl: public virtual Label_pskel,
    public Item_pimpl
{
public:
    virtual void pre();
    virtual void text(const ::std::string &);
    virtual void color(const QColor&);
    virtual void font_family(const ::std::string &);
    virtual void font_size(unsigned long long);
    virtual LabelItem *post_Label();

private:
    QString m_text;
    QString m_color;
    QString m_fontFamily;
    int m_fontSize;
};

class Font_pimpl: public virtual Font_pskel
{
public:
    virtual void pre();
    virtual void family(const ::std::string &);
    virtual void size(unsigned long long);
    virtual void bold(bool);
    virtual void italic(bool);
    virtual void underline(bool);
    virtual void strikeout(bool);
    virtual QFont post_Font();

private:
    QFont m_font;
};

class Point_pimpl: public virtual Point_pskel
{
public:
    virtual void pre();
    virtual void x(double);
    virtual void y(double);
    virtual QPointF post_Point();

private:
    qreal m_x;
    qreal m_y;
};

class PointList_pimpl: public virtual PointList_pskel
{
public:
    virtual void pre();
    virtual void point(const QPointF &);
    virtual QList<QPointF> post_PointList();

private:
    QList<QPointF> m_points;
};

class Pen_pimpl: public virtual Pen_pskel
{
public:
    virtual void pre();
    virtual void width(const qreal &);
    virtual void color(const QColor &);
    virtual void style(const Qt::PenStyle &);
    virtual void cap_style(const Qt::PenCapStyle &);
    virtual void join_style(const Qt::PenJoinStyle &);
    virtual QPen post_Pen();

private:
    QPen m_pen;
};

class Brush_pimpl: public virtual Brush_pskel
{
public:
    virtual void pre();
    virtual void color(const QColor &);
    virtual void style(const Qt::BrushStyle &);
    virtual QBrush post_Brush();

private:
    QBrush m_brush;
};

class PenStyle_pimpl: public virtual PenStyle_pskel,
    public ::xml_schema::string_pimpl
{
public:
    virtual void pre();
    virtual Qt::PenStyle post_PenStyle();
};

class PenCapStyle_pimpl: public virtual PenCapStyle_pskel,
    public ::xml_schema::string_pimpl
{
public:
    virtual void pre();
    virtual Qt::PenCapStyle post_PenCapStyle();
};

class PenJoinStyle_pimpl: public virtual PenJoinStyle_pskel,
    public ::xml_schema::string_pimpl
{
public:
    virtual void pre();
    virtual Qt::PenJoinStyle post_PenJoinStyle();
};

class BrushStyle_pimpl: public virtual BrushStyle_pskel,
    public ::xml_schema::string_pimpl
{
public:
    virtual void pre();
    virtual Qt::BrushStyle post_BrushStyle();
};

class Color_pimpl: public virtual Color_pskel,
    public ::xml_schema::string_pimpl
{
public:
    virtual void pre();
    virtual QColor post_Color();
};
class Angle_pimpl: public virtual Angle_pskel,
    public ::xml_schema::double_pimpl
{
public:
    virtual void pre();
    virtual qreal post_Angle();
};

class Opacity_pimpl: public virtual Opacity_pskel,
    public ::xml_schema::double_pimpl
{
public:
    virtual void pre();
    virtual qreal post_Opacity();
};

class NonNegativeDouble_pimpl: public virtual NonNegativeDouble_pskel,
    public ::xml_schema::double_pimpl
{
public:
    virtual void pre();
    virtual qreal post_NonNegativeDouble();
};

}
}

#endif // LEDA_XDL_SYMBOL_PIMPL_HXX
