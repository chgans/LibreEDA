#include "leda-xdl-symbol-pimpl.h"

namespace xdl {
namespace symbol {

// Black pen, cosmetic (width = 0.0), solid line with round joins and round cap
static const QPen DEFAUL_PEN(QBrush(Qt::black), 0.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
static const QBrush DEFAULT_BRUSH(Qt::NoBrush);


// Symbol_pimpl
//

void Symbol_pimpl::pre()
{
    m_name.clear();
    m_label.clear();
    m_drawing.clear();
}

void Symbol_pimpl::name(const ::std::string &name)
{
    m_name = QString::fromStdString(name);
}

void Symbol_pimpl::label(const ::std::string &label)
{
    m_label = QString::fromStdString(label);
}

void Symbol_pimpl::drawing(const QList<Item *> &drawing)
{
    m_drawing = drawing;
}

Symbol *Symbol_pimpl::post_Symbol()
{
    auto symbol = new Symbol();
    symbol->name = m_name;
    symbol->description = m_label; // FIXME
    symbol->designatorPrefix = "X"; // FIXME
    symbol->drawingItems = m_drawing;
    return symbol;
}

// ItemList_pimpl
//

void ItemList_pimpl::pre()
{
    m_items.clear();
}

void ItemList_pimpl::polyline(PolylineItem *polyline)
{
    m_items.append(polyline);
}

void ItemList_pimpl::polygon(PolygonItem *polygon)
{
    m_items.append(polygon);
}

void ItemList_pimpl::rectangle(RectangleItem *rectangle)
{
    m_items.append(rectangle);
}

void ItemList_pimpl::circle(CircleItem *circle)
{
    m_items.append(circle);
}

void ItemList_pimpl::circular_arc(CircularArcItem *circular_arc)
{
    m_items.append(circular_arc);
}

void ItemList_pimpl::ellipse(EllipseItem *ellipse)
{
    m_items.append(ellipse);
}

void ItemList_pimpl::elliptical_arc(EllipticalArcItem *elliptical_arc)
{
    m_items.append(elliptical_arc);
}

void ItemList_pimpl::label(LabelItem *label)
{
    m_items.append(label);
}

void ItemList_pimpl::pin(PinItem *pin)
{
    m_items.append(pin);
}

void ItemList_pimpl::group(ItemGroup *group)
{
    m_items.append(group);
}

QList<symbol::Item *> ItemList_pimpl::post_ItemList()
{
    return m_items;
}

// Item_pimpl
//

void Item_pimpl::pre()
{
    m_pen = DEFAUL_PEN;
    m_brush = DEFAULT_BRUSH;
    m_position = QPointF();
    m_zValue = 0.0;
    m_rotation = 0.0;
    m_opacity = 1.0;
    m_xMirrored = false;
    m_yMirrored = false;
    m_locked = false;
    m_visible = true;
}

void Item_pimpl::pen(const QPen &pen)
{
    m_pen = pen;
}

void Item_pimpl::brush(const QBrush &brush)
{
    m_brush = brush;
}

void Item_pimpl::position(const QPointF &position)
{
    m_position = position;
}

void Item_pimpl::z_value(double z_value)
{
    m_zValue = z_value;
}

void Item_pimpl::rotation(const qreal &rotation)
{
    m_rotation = rotation;
}

void Item_pimpl::opacity(const qreal &opacity)
{
    m_opacity = opacity;
}

void Item_pimpl::locked(bool locked)
{
    m_locked = locked;
}

void Item_pimpl::x_mirrored(bool x_mirrored)
{
    m_xMirrored = x_mirrored;
}

void Item_pimpl::y_mirrored(bool y_mirrored)
{
    m_yMirrored = y_mirrored;
}

void Item_pimpl::visible(bool visible)
{
    m_visible = visible;
}

void Item_pimpl::post_Item()
{
    m_item->setLineStyle(LineStyle(m_pen.style()));
    m_item->setLineWidth(MediumLine); // FIXME
    m_item->setLineColor(m_pen.color().name());
    m_item->setFillColor(m_brush.color().name());
    m_item->setPosition(m_position);
    //m_item->zValue = m_zValue;
    m_item->setRotation(m_rotation);
    m_item->setOpacity(m_opacity);
    m_item->setLocked(m_locked);
    m_item->setXMirrored(m_xMirrored);
    m_item->setYMirrored(m_yMirrored);
    m_item->setVisible(m_visible);
}

// Circle_pimpl
//

void Circle_pimpl::pre()
{
    Item_pimpl::pre();
    m_radius = 0.0f;
}

void Circle_pimpl::radius(const qreal &radius)
{
    m_radius = radius;
}

CircleItem *Circle_pimpl::post_Circle()
{
    auto *circle = new CircleItem();
    m_item = circle;
    post_Item();
    circle->setRadius(m_radius);
    return circle;
}

// CircularArc_pimpl
//

void CircularArc_pimpl::pre()
{
    Item_pimpl::pre();
    m_radius = 0.0;
    m_startAngle = 0.0;
    m_spanAngle = 360.0;
}

void CircularArc_pimpl::radius(const qreal &radius)
{
    m_radius = radius;
}

void CircularArc_pimpl::start_angle(const qreal &start_angle)
{
    m_startAngle = start_angle;
}

void CircularArc_pimpl::span_angle(const qreal &span_angle)
{
    m_spanAngle = span_angle;
}

CircularArcItem *CircularArc_pimpl::post_CircularArc()
{
    auto arc = new CircularArcItem();
    m_item = arc;
    post_Item();
    arc->setRadius(m_radius);
    //arc->center = m_center;
    arc->setStartAngle(m_startAngle);
    arc->setSpanAngle(m_spanAngle);
    return arc;
}

// Ellipse_pimpl
//

void Ellipse_pimpl::pre()
{
    Item_pimpl::pre();
    m_xRadius = 0.0;
    m_yRadius = 0.0;
}

void Ellipse_pimpl::x_radius(const qreal &x_radius)
{
    m_xRadius = x_radius;
}

void Ellipse_pimpl::y_radius(const qreal &y_radius)
{
    m_yRadius = y_radius;
}

EllipseItem *Ellipse_pimpl::post_Ellipse()
{
    auto *ellipse = new EllipseItem();
    m_item = ellipse;
    post_Item();
    ellipse->setXRadius(m_xRadius);
    ellipse->setYRadius(m_yRadius);
    //ellipse->center = m_center;
    return ellipse;
}

// EllipticalArc_pimpl
//

void EllipticalArc_pimpl::pre()
{
    Item_pimpl::pre();
    m_xRadius = 0.0;
    m_yRadius = 0.0;
    m_startAngle = 0.0;
    m_spanAngle = 360.0;
}

void EllipticalArc_pimpl::x_radius(const qreal &x_radius)
{
    m_xRadius = x_radius;
}

void EllipticalArc_pimpl::y_radius(const qreal &y_radius)
{
    m_yRadius = y_radius;
}

void EllipticalArc_pimpl::start_angle(const qreal &start_angle)
{
    m_startAngle = start_angle;
}

void EllipticalArc_pimpl::span_angle(const qreal &span_angle)
{
    m_spanAngle = span_angle;
}

EllipticalArcItem *EllipticalArc_pimpl::post_EllipticalArc()
{
    auto *arc = new EllipticalArcItem();
    m_item = arc;
    post_Item();
    arc->setXRadius(m_xRadius);
    arc->setYRadius(m_yRadius);
    //arc->center = m_center;
    arc->setStartAngle(m_startAngle);
    arc->setSpanAngle(m_spanAngle);
    return arc;
}

// Rectangle_pimpl
//

void Rectangle_pimpl::pre()
{
    Item_pimpl::pre();
    m_width = 0.0;
    m_height = 0.0;
}

void Rectangle_pimpl::width(const qreal &width)
{
    m_width = width;
}

void Rectangle_pimpl::height(const qreal &height)
{
    m_height = height;
}


symbol::RectangleItem *Rectangle_pimpl::post_Rectangle()
{
    auto rect  = new RectangleItem();
    m_item = rect;
    post_Item();
    rect->setWidth(m_width);
    rect->setHeight(m_height);
    return rect;
}

// Polyline_pimpl
//

void Polyline_pimpl::pre()
{
    Item_pimpl::pre();
    m_vertices.clear();
}

void Polyline_pimpl::vertices(const QList<QPointF> &points)
{
    m_vertices = points;
}

PolylineItem *Polyline_pimpl::post_Polyline()
{
    auto *polyline = new PolylineItem();
    m_item = polyline;
    post_Item();
    polyline->setVertices(m_vertices);
    return polyline;
}

// Polygon_pimpl
//

void Polygon_pimpl::pre()
{
    Item_pimpl::pre();
    m_vertices.clear();
}

void Polygon_pimpl::vertices(const QList<QPointF> &vertices)
{
    m_vertices = vertices;
}

PolygonItem *Polygon_pimpl::post_Polygon()
{
    auto *polygon = new PolygonItem();
    m_item = polygon;
    post_Item();
    polygon->setVertices(m_vertices);
    return polygon;
}

// Pin_pimpl
//

void Pin_pimpl::pre()
{
    Item_pimpl::pre();
    m_designator.clear();
    m_label.clear();
}

void Pin_pimpl::designator(const ::std::string &designator)
{
    m_designator = QString::fromStdString(designator);
}

void Pin_pimpl::label(const ::std::string &label)
{
    m_label = QString::fromStdString(label);;
}

PinItem *Pin_pimpl::post_Pin()
{
    auto *pin = new PinItem();
    m_item = pin;
    post_Item();
    pin->designator = new LabelItem();
    //pin->designator->text = m_designator; // FIXME
    pin->label = new LabelItem();
    //pin->label->text = m_label; // FIXME
    return pin;
}

// ItemGroup_pimpl
//

void ItemGroup_pimpl::pre()
{
    m_children.clear();
}

void ItemGroup_pimpl::children(const QList<Item *> &children)
{
    m_children = children;
}

ItemGroup *ItemGroup_pimpl::post_ItemGroup()
{
    auto *group = new ItemGroup();
    m_item = group;
    post_Item();
    group->children = m_children;
    return group;
}


// Label_pimpl
//

void Label_pimpl::pre()
{
    Item_pimpl::pre();
    m_fontFamily = ""; // FIXME
    m_text.clear();
    m_color = "#000000";
    // Override default shape pen/brush
    m_pen = QPen(Qt::NoPen);
    m_brush = QBrush(Qt::black, Qt::SolidPattern);
}

void Label_pimpl::text(const std::string &text)
{
    m_text = QString::fromStdString(text);
}

void Label_pimpl::color(const QColor &color)
{
    m_color = color.name();
}

void Label_pimpl::font_family(const std::string &family)
{
    m_fontFamily = QString::fromStdString(family);
}

void Label_pimpl::font_size(unsigned long long size)
{
    m_fontSize = size;
}

LabelItem *Label_pimpl::post_Label()
{
    auto *label = new LabelItem();
    m_item = label;
    post_Item();
    label->setText(m_text);
    label->setTextColor(m_color);
    label->setFontFamily(m_fontFamily);
    label->setFontSize(m_fontSize);
    return label;
}

// Point_pimpl
//

void Point_pimpl::pre()
{
    m_x = 0.0;
    m_y = 0.0;
}

void Point_pimpl::x(double x)
{
    m_x = x;
}

void Point_pimpl::y(double y)
{
    m_y = y;
}

QPointF Point_pimpl::post_Point()
{
    return QPointF(m_x, m_y);
}

// PointList_pimpl
//

void PointList_pimpl::pre()
{
    m_points.clear();
}

void PointList_pimpl::point(const QPointF &point)
{
    m_points.append(point);
}

QList<QPointF> PointList_pimpl::post_PointList()
{
    return m_points;
}

// Pen_pimpl
//

void Pen_pimpl::pre()
{
    m_pen = DEFAUL_PEN;
}

void Pen_pimpl::width(const qreal &width)
{
    m_pen.setWidthF(width);
}

void Pen_pimpl::color(const QColor &color)
{
    m_pen.setColor(color);
}

void Pen_pimpl::style(const Qt::PenStyle &style)
{
    m_pen.setStyle(style);
}

void Pen_pimpl::cap_style(const Qt::PenCapStyle &cap_style)
{
    m_pen.setCapStyle(cap_style);
}

void Pen_pimpl::join_style(const Qt::PenJoinStyle &join_style)
{
    m_pen.setJoinStyle(join_style);
}

QPen Pen_pimpl::post_Pen()
{
    return m_pen;
}

// Brush_pimpl
//

void Brush_pimpl::pre()
{
    m_brush = DEFAULT_BRUSH;
}

void Brush_pimpl::color(const QColor &color)
{
    m_brush.setColor(color);
}

void Brush_pimpl::style(const Qt::BrushStyle &style)
{
    m_brush.setStyle(style);
}

QBrush Brush_pimpl::post_Brush()
{
    return m_brush;
}

// PenStyle_pimpl
//

void PenStyle_pimpl::pre()
{
}

Qt::PenStyle PenStyle_pimpl::post_PenStyle()
{
    const ::std::string &v(post_string());
    if (v == "NoPen")
    {
        return Qt::NoPen;
    }
    if (v == "SolidLine")
    {
        return Qt::SolidLine;
    }
    if (v == "DashLine")
    {
        return Qt::DashLine;
    }
    if (v == "DashDotLine")
    {
        return Qt::DashDotLine;
    }
    if (v == "DashDotdotLine")
    {
        return Qt::DashDotDotLine;
    }
    return Qt::SolidLine;
}

// PenCapStyle_pimpl
//

void PenCapStyle_pimpl::pre()
{
}

Qt::PenCapStyle PenCapStyle_pimpl::post_PenCapStyle()
{
    const ::std::string &v(post_string());
    if (v == "Flat")
    {
        return Qt::FlatCap;
    }
    if (v == "Square")
    {
        return Qt::SquareCap;
    }
    if (v == "Round")
    {
        return Qt::RoundCap;
    }
    return Qt::SquareCap;
}

// PenJoinStyle_pimpl
//

void PenJoinStyle_pimpl::pre()
{
}

Qt::PenJoinStyle PenJoinStyle_pimpl::post_PenJoinStyle()
{
    const ::std::string &v(post_string());
    if (v == "Miter")
    {
        return Qt::MiterJoin;
    }
    if (v == "Bevel")
    {
        return Qt::BevelJoin;
    }
    if (v == "Round")
    {
        return Qt::RoundJoin;
    }
    return Qt::RoundJoin;
}

// BrushStyle_pimpl
//

void BrushStyle_pimpl::pre()
{
}

Qt::BrushStyle BrushStyle_pimpl::post_BrushStyle()
{
    const ::std::string &v(post_string());
    if (v == "NoBrush")
    {
        return Qt::NoBrush;
    }
    if (v == "Solid")
    {
        return Qt::SolidPattern;
    }
    if (v == "Horizontal")
    {
        return Qt::HorPattern;
    }
    if (v == "Vertical")
    {
        return Qt::VerPattern;
    }
    if (v == "Cross")
    {
        return Qt::CrossPattern;
    }
    if (v == "BDiagonal")
    {
        return Qt::BDiagPattern;
    }
    if (v == "FDiagonal")
    {
        return Qt::FDiagPattern;
    }
    if (v == "CrossDiagonal")
    {
        return Qt::DiagCrossPattern;
    }
    return Qt::SolidPattern;
}

// Color_pimpl
//

void Color_pimpl::pre()
{
}

QColor Color_pimpl::post_Color()
{
    return QColor(QString::fromStdString(post_string()));
}

// Angle_pimpl
//

void Angle_pimpl::pre()
{
}

qreal Angle_pimpl::post_Angle()
{
    return post_double();
}

// Opacity_pimpl
//

void Opacity_pimpl::pre()
{
}

qreal Opacity_pimpl::post_Opacity()
{
    return post_double();
}

// NonNegativeDouble_pimpl
//

void NonNegativeDouble_pimpl::pre()
{
}

qreal NonNegativeDouble_pimpl::post_NonNegativeDouble()
{
    return post_double();
}

void Font_pimpl::pre()
{
}

void Font_pimpl::family(const std::string &family)
{
    m_font.setFamily(QString::fromStdString(family));
}

void Font_pimpl::size(unsigned long long size)
{
    m_font.setPointSize(size);
}

void Font_pimpl::bold(bool bold)
{
    m_font.setBold(bold);
}

void Font_pimpl::italic(bool italic)
{
    m_font.setItalic(italic);
}

void Font_pimpl::underline(bool underline)
{
    m_font.setUnderline(underline);
}

void Font_pimpl::strikeout(bool strikeOut)
{
    m_font.setStrikeOut(strikeOut);
}

QFont Font_pimpl::post_Font()
{
    return m_font;
}

}
}
