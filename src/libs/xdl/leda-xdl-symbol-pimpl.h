// Not copyrighted - public domain.
//
// This sample parser implementation was generated by CodeSynthesis XSD,
// an XML Schema to C++ data binding compiler. You may use it in your
// programs without any restrictions.
//

#ifndef LEDA_XDL_SYMBOL_PIMPL_HXX
#define LEDA_XDL_SYMBOL_PIMPL_HXX

#include "leda-xdl-symbol-pskel.hxx"

namespace xdl
{

class Symbol_pimpl: public virtual Symbol_pskel
{
public:
    virtual void pre();
    virtual void name(const ::std::string&);
    virtual void label(const ::std::string&);
    virtual void drawing(QList<QGraphicsItem*>);
    virtual Symbol* post_Symbol();

private:
    QString m_name;
    QString m_label;
    QList<QGraphicsItem*> m_drawing;
};

class ItemList_pimpl: public virtual ItemList_pskel
{
public:
    virtual void pre();
    virtual void ellipse(QGraphicsEllipseItem*);
    virtual void line(QGraphicsLineItem*);
    virtual void pin(QGraphicsEllipseItem*);
    virtual void rectangle(QGraphicsRectItem*);
    virtual void group(QGraphicsItemGroup*);
    virtual void arc(QGraphicsPathItem *);
    virtual void label(QGraphicsSimpleTextItem *);
    virtual QList<QGraphicsItem*> post_ItemList();

private:
    QList<QGraphicsItem*> m_items;
};

class Item_pimpl: public virtual Item_pskel
{
public:
    virtual void pre();
    virtual void position(const QPointF&);
    virtual void z_value(double);
    virtual void rotation(const qreal&);
    virtual void opacity(const qreal&);
    virtual void locked(bool);
    virtual void mirrored(const QTransform&);
    virtual void visible(bool);
    virtual void post_Item();

protected:
    QGraphicsItem *m_item;

private:
    QPointF m_position;
    qreal m_zValue;
    qreal m_rotation;
    qreal m_opacity;
    bool m_locked;
    QTransform m_transform;
    bool m_visible;
};

class Shape_pimpl: public virtual Shape_pskel,
        public ::xdl::Item_pimpl
{
public:
    virtual void pre();
    virtual void pen(const QPen&);
    virtual void brush(const QBrush&);
    virtual void post_Shape();

protected:
    QAbstractGraphicsShapeItem *m_shapeItem;
    QPen m_pen;
    QBrush m_brush;
};

class Ellipse_pimpl: public virtual Ellipse_pskel,
        public ::xdl::Shape_pimpl
{
public:
    virtual void pre();
    virtual void center(const QPointF&);
    virtual void x_radius(const qreal&);
    virtual void y_radius(const qreal&);
    virtual void start_angle(const qreal&);
    virtual void span_angle(const qreal&);
    virtual QGraphicsEllipseItem *post_Ellipse();

private:
    QPointF m_center;
    qreal m_xRadius;
    qreal m_yRadius;
    qreal m_startAngle;
    qreal m_spanAngle;
};

class Rectangle_pimpl: public virtual Rectangle_pskel,
        public ::xdl::Shape_pimpl
{
public:
    virtual void pre();
    virtual void top_left(const QPointF&);
    virtual void bottom_right(const QPointF&);
    virtual QGraphicsRectItem *post_Rectangle();

private:
    QPointF m_topLeft;
    QPointF m_bottomRight;
};

class Line_pimpl: public virtual Line_pskel,
        public ::xdl::Item_pimpl
{
public:
    virtual void pre();
    virtual void points(const QList<QPointF>&);
    virtual void pen(const QPen&);
    virtual QGraphicsLineItem *post_Line();

private:
    QLineF m_line;
    QPen m_pen;
};

class Pin_pimpl: public virtual Pin_pskel,
        public ::xdl::Item_pimpl
{
public:
    virtual void pre();
    virtual void designator(const ::std::string&);
    virtual QGraphicsEllipseItem *post_Pin();

private:
    QString m_designator;
};

class ItemGroup_pimpl: public virtual ItemGroup_pskel,
        public ::xdl::Item_pimpl
{
public:
    virtual void pre();
    virtual void children(QList<QGraphicsItem*>);
    virtual QGraphicsItemGroup *post_ItemGroup();

private:
    QList<QGraphicsItem*> m_children;
};

class Arc_pimpl: public virtual Arc_pskel,
        public ::xdl::Shape_pimpl
{
public:
    virtual void pre();
    virtual void center(const QPointF&);
    virtual void x_radius(const qreal&);
    virtual void y_radius(const qreal&);
    virtual void start_angle(const qreal&);
    virtual void span_angle(const qreal&);
    virtual QGraphicsPathItem *post_Arc();

private:
    QPointF m_center;
    qreal m_xRadius;
    qreal m_yRadius;
    qreal m_startAngle;
    qreal m_spanAngle;
};

class Label_pimpl: public virtual Label_pskel,
        public Shape_pimpl
{
public:
    virtual void pre();
    virtual void text(const ::std::string&);
    virtual void font(const QFont&);
    virtual QGraphicsSimpleTextItem* post_Label();

private:
    QString m_text;
    QFont m_font;
};

class Font_pimpl: public virtual Font_pskel
{
public:
    virtual void pre();
    virtual void family(const ::std::string&);
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
    virtual void point(const QPointF&);
    virtual QList<QPointF> post_PointList();

private:
    QList<QPointF> m_points;
};

class Pen_pimpl: public virtual Pen_pskel
{
public:
    virtual void pre();
    virtual void width(const qreal &);
    virtual void color(const QColor&);
    virtual void style(const Qt::PenStyle&);
    virtual void cap_style(const Qt::PenCapStyle&);
    virtual void join_style(const Qt::PenJoinStyle&);
    virtual QPen post_Pen();

private:
    QPen m_pen;
};

class Brush_pimpl: public virtual Brush_pskel
{
public:
    virtual void pre();
    virtual void color(const QColor&);
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

class Side_pimpl: public virtual Side_pskel,
        public ::xml_schema::string_pimpl
{
public:
    virtual void pre();
    virtual void post_Side();
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

class Mirroring_pimpl: public virtual Mirroring_pskel
{
public:
    virtual void pre();
    virtual void x(bool);
    virtual void y(bool);
    virtual QTransform post_Mirroring();

private:
    bool m_xScale;
    bool m_yScale;
};

class NonNegativeDouble_pimpl: public virtual NonNegativeDouble_pskel,
        public ::xml_schema::double_pimpl
{
public:
    virtual void pre();
    virtual qreal post_NonNegativeDouble();
};


class PinSymbol_pimpl: public virtual PinSymbol_pskel
{
public:
    virtual void pre() {}
    virtual void type() {}
    virtual void location() {}
    virtual void post_PinSymbol() {}
};

class PinSymbolList_pimpl: public virtual PinSymbolList_pskel
{
public:
    virtual void pre() {}
    virtual void symbol() {}
    virtual void post_PinSymbolList() {}
};

class PinSymbolEnum_pimpl: public virtual PinSymbolEnum_pskel,
        public ::xml_schema::string_pimpl
{
public:
    virtual void pre() {}
    virtual void post_PinSymbolEnum() {}
};
}

#endif // LEDA_XDL_SYMBOL_PIMPL_HXX
