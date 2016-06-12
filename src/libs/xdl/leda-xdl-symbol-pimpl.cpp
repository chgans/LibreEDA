#include "leda-xdl-symbol-pimpl.h"

namespace xdl {
    namespace symbol {

        static const LineStyle DEFAULT_LINE_STYLE = SolidLine;
        static const LineWidth DEFAULT_LINE_WIDTH = MediumLine;
        static const Color DEFAULT_LINE_COLOR = PrimaryContent;
        static const FillStyle DEFAULT_FILL_STYLE = SolidFill;
        static const Color DEFAULT_FILL_COLOR = Background;
        static const int DEFAULT_TEXT_SIZE = 12;
        static const Color DEFAULT_TEXT_COLOR = PrimaryContent;


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
            m_lineStyle = DEFAULT_LINE_STYLE;
            m_lineWidth = DEFAULT_LINE_WIDTH;
            m_lineColor = DEFAULT_LINE_COLOR;
            m_fillStyle = DEFAULT_FILL_STYLE;
            m_fillColor = DEFAULT_FILL_COLOR;
            m_position = QPointF();
            m_zValue = 0.0;
            m_rotation = 0.0;
            m_opacity = 1.0;
            m_xMirrored = false;
            m_yMirrored = false;
            m_locked = false;
            m_visible = true;
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
            m_item->setLineStyle(m_lineStyle);
            m_item->setLineWidth(m_lineWidth);
            m_item->setLineColor(m_lineColor);
            m_item->setFillStyle(m_fillStyle);
            m_item->setFillColor(m_fillColor);
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
            m_text.clear();
            m_color = DEFAULT_TEXT_COLOR;
            m_fontSize = DEFAULT_TEXT_SIZE;
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

        // LineWidth_pimpl
        //

        void LineWidth_pimpl::pre ()
        {
        }

        LineWidth LineWidth_pimpl::post_LineWidth ()
        {
            const ::std::string& v (post_string ());

            if (v == "Thinest")
            {
                return ThinestLine;
            }
            if (v == "Thiner")
            {
                return ThinerLine;
            }
            if (v == "SlightlyThin")
            {
                return SlightlyThinLine;
            }
            if (v == "Medium")
            {
                return MediumLine;
            }
            if (v == "SlightltThick")
            {
                return SlightlyThickLine;
            }
            if (v == "Thicker")
            {
                return ThickerLine;
            }
            if (v == "Thickest")
            {
                return ThickestLine;
            }
            return MediumLine;
        }

        // LineStyle_pimpl
        //

        void LineStyle_pimpl::pre ()
        {
        }

        LineStyle LineStyle_pimpl::post_LineStyle ()
        {
            const ::std::string &v(post_string());
            if (v == "NoLine")
            {
                return NoLine;
            }
            if (v == "SolidLine")
            {
                return SolidLine;
            }
            if (v == "DashLine")
            {
                return DashLine;
            }
            if (v == "DashDotLine")
            {
                return DashDotLine;
            }
            if (v == "DashDotdotLine")
            {
                return DashDotDotLine;
            }
            return SolidLine;
        }

        // FillStyle_pimpl
        //

        void FillStyle_pimpl::pre ()
        {
        }

        FillStyle FillStyle_pimpl::post_FillStyle ()
        {
            const ::std::string& v (post_string ());
            if (v == "NoFill")
            {
                return NoFill;
            }
            if (v == "SolidFill")
            {
                return SolidFill;
            }
            return SolidFill;
        }

        // Color_pimpl
        //

        void Color_pimpl::pre ()
        {
        }

        Color Color_pimpl::post_Color ()
        {
            const ::std::string& v (post_string ());
            if (v == "PrimaryContent")
            {
                return PrimaryContent;
            }
            if (v == "SecondaryContent")
            {
                return SecondaryContent;
            }
            if (v == "EmphasisedContent")
            {
                return EmphasisedContent;
            }
            if (v == "Background")
            {
                return Background;
            }
            if (v == "BackgroundHighlight")
            {
                return BackgroundHighlight;
            }
            if (v == "Yellow")
            {
                return Yellow;
            }
            if (v == "Orange")
            {
                return Orange;
            }
            if (v == "Red")
            {
                return Red;
            }
            if (v == "Magenta")
            {
                return Magenta;
            }
            if (v == "Violet")
            {
                return Violet;
            }
            if (v == "Blue")
            {
                return Blue;
            }
            if (v == "Cyan")
            {
                return Cyan;
            }
            if (v == "Green")
            {
                return Green;
            }
            return PrimaryContent;
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

    }
}
