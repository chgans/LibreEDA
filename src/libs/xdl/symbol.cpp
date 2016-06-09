#include "symbol.h"

namespace xdl
{
    namespace symbol
    {

        Item::Item():
            m_lineStyle(SolidLine),
            m_lineWidth(MediumLine),
            m_lineColor("#000000"),
            m_fillColor("#000000"), // FIXME: No fill
            m_opacity(1.0),
            m_position(QPointF(0.0, 0.0)),
            m_rotation(0.0),
            m_locked(false),
            m_visible(true),
            m_xMirrored(false),
            m_yMirrored(false)
        {

        }

        Item::~Item()
        {

        }

        QString Item::friendlyPropertyName(quint64 id) const
        {
            switch (id)
            {
                case PositionProperty:
                    return "Position";
                case OpacityProperty:
                    return "Opacity";
                case RotationProperty:
                    return "Rotation";
                case LockedProperty:
                    return "Locked";
                case VisibilityProperty:
                    return "Visibility";
                case XMirroredProperty:
                    return "X Mirroring";
                case YMirroredProperty:
                    return "Y Mirroring";
                case LineStyleProperty:
                    return "Line style";
                case LineWidthProperty:
                    return "Line width";
                case LineColorProperty:
                    return "Line Color";
                case FillColorProperty:
                    return "Fill color";
                default:
                    return "Unknown";
            }
        }

        QVariant Item::property(int id)
        {
            switch (id)
            {
                case PositionProperty:
                    return m_position;
                case OpacityProperty:
                    return m_opacity;
                case RotationProperty:
                    return m_rotation;
                case LockedProperty:
                    return m_locked;
                case VisibilityProperty:
                    return m_visible;
                case XMirroredProperty:
                    return m_xMirrored;
                case YMirroredProperty:
                    return m_yMirrored;
                case LineStyleProperty:
                    return m_lineStyle;
                case LineWidthProperty:
                    return m_lineWidth;
                case LineColorProperty:
                    return m_lineColor;
                case FillColorProperty:
                    return m_fillColor;
                default:
                    return QVariant();
            }
        }

        void Item::setProperty(int id, const QVariant &value)
        {
            switch (id)
            {
                case PositionProperty:
                    m_position = value.toPointF();
                    break;
                case OpacityProperty:
                    m_opacity = value.toReal();
                    break;
                case RotationProperty:
                    m_rotation = value.toReal();
                    break;
                case LockedProperty:
                    m_locked = value.toBool();
                    break;
                case VisibilityProperty:
                    m_visible = value.toBool();
                    break;
                case XMirroredProperty:
                    m_xMirrored = value.toBool();
                    break;
                case YMirroredProperty:
                    m_yMirrored = value.toBool();
                    break;
                case LineStyleProperty:
                    m_lineStyle = LineStyle(value.toInt());
                    break;
                case LineWidthProperty:
                    m_lineWidth = LineWidth(value.toInt());
                    break;
                case LineColorProperty:
                    m_lineColor = value.toString();
                    break;
                case FillColorProperty:
                    m_fillColor = value.toString();
                    break;
                default:
                    break;
            }
        }

        void Item::setPosition(const QPointF &pos)
        {
            m_position = pos;
        }

        QPointF Item::position() const
        {
            return m_position;
        }

        void Item::setOpacity(qreal opacity)
        {
            m_opacity = opacity;
        }

        qreal Item::opacity() const
        {
            return m_opacity;
        }

        void Item::setRotation(qreal rotation)
        {
            m_rotation = rotation;
        }

        qreal Item::rotation() const
        {
            return m_rotation;
        }

        void Item::setLocked(bool locked)
        {
            m_locked = locked;
        }

        bool Item::isLocked() const
        {
            return m_locked;
        }

        void Item::setVisible(bool visible)
        {
            m_visible = visible;
        }

        bool Item::isVisible() const
        {
            return m_visible;
        }

        void Item::setXMirrored(bool mirrored)
        {
            m_xMirrored = mirrored;
        }

        bool Item::isXMirrored() const
        {
            return m_xMirrored;
        }

        void Item::setYMirrored(bool mirrored)
        {
            m_yMirrored = mirrored;
        }

        bool Item::isYMirrored() const
        {
            return m_yMirrored;
        }

        void Item::setLineStyle(LineStyle style)
        {
            m_lineStyle = style;
        }

        LineStyle Item::lineStyle() const
        {
            return m_lineStyle;
        }

        void Item::setLineWidth(LineWidth width)
        {
            m_lineWidth = width;
        }

        LineWidth Item::lineWidth() const
        {
            return m_lineWidth;
        }

        void Item::setLineColor(const QString &color)
        {
            m_lineColor = color;
        }

        QString Item::lineColor() const
        {
            return m_lineColor;
        }

        void Item::setFillColor(const QString &color)
        {
            m_fillColor = color;
        }

        QString Item::fillColor() const
        {
            return m_fillColor;
        }

        Symbol::Symbol()
        {

        }

        Symbol::~Symbol()
        {
            //qDeleteAll(drawingItems); // We don't have ownership
        }

        RectangleItem::RectangleItem():
            m_width(0.0),
            m_height(0.0)
        {

        }

        RectangleItem::~RectangleItem()
        {

        }

        qreal RectangleItem::width() const
        {
            return m_width;
        }

        void RectangleItem::setWidth(qreal width)
        {
            m_width = width;
        }

        qreal RectangleItem::height() const
        {
            return m_height;
        }

        void RectangleItem::setHeight(qreal height)
        {
            m_height = height;
        }

        Item::Type RectangleItem::type() const
        {
            return Rectangle;
        }

        Item *RectangleItem::clone() const
        {
            auto item = new RectangleItem;
            *item = *this;
            return item;
        }

        QString RectangleItem::friendlyTypeName() const
        {
            return "Rectangle";
        }

        QString RectangleItem::friendlyPropertyName(quint64 id) const
        {
            switch (id)
            {
                case WidthProperty:
                    return "Width";
                case HeightProperty:
                    return "Height";
                default:
                    return Item::friendlyPropertyName(id);
            }
        }

        QIcon RectangleItem::icon() const
        {
            return QIcon::fromTheme("draw-rectangle");
        }

        QVariant RectangleItem::property(int id)
        {
            switch (id)
            {
                case WidthProperty:
                    return m_width;
                case HeightProperty:
                    return m_height;
                default:
                    return Item::property(id);
            }
        }

        void RectangleItem::setProperty(int id, const QVariant &value)
        {
            switch (id)
            {
                case WidthProperty:
                    m_width = value.toReal();
                    break;
                case HeightProperty:
                    m_height = value.toReal();
                    break;
                default:
                    Item::setProperty(id, value);
                    break;
            }
        }

        CircleItem::CircleItem():
            m_radius(0.0)
        {

        }

        CircleItem::~CircleItem()
        {

        }

        qreal CircleItem::radius() const
        {
            return m_radius;
        }

        void CircleItem::setRadius(qreal radius)
        {
            m_radius = radius;
        }

        Item::Type CircleItem::type() const
        {
            return Circle;
        }

        Item *CircleItem::clone() const
        {
            auto item = new CircleItem;
            *item = *this;
            return item;
        }

        QString CircleItem::friendlyTypeName() const
        {
            return "Circle";
        }

        QString CircleItem::friendlyPropertyName(quint64 id) const
        {
            switch (id)
            {
                case RadiusProperty:
                    return "Radius";
                default:
                    return Item::friendlyPropertyName(id);
            }
        }

        QIcon CircleItem::icon() const
        {
            return QIcon::fromTheme("draw-circle");
        }

        QVariant CircleItem::property(int id)
        {
            switch (id)
            {
                case RadiusProperty:
                    return m_radius;
                default:
                    return Item::property(id);
            }
        }

        void CircleItem::setProperty(int id, const QVariant &value)
        {
            switch (id)
            {
                case RadiusProperty:
                    m_radius = value.toReal();
                    break;
                default:
                    Item::setProperty(id, value);
                    break;
            }
        }

        CircularArcItem::CircularArcItem():
            m_radius(0.0),
            m_startAngle(0.0),
            m_spanAngle(360.0)
        {

        }

        CircularArcItem::~CircularArcItem()
        {

        }

        qreal CircularArcItem::radius() const
        {
            return m_radius;
        }

        void CircularArcItem::setRadius(qreal radius)
        {
            m_radius = radius;
        }

        qreal CircularArcItem::startAngle() const
        {
            return m_startAngle;
        }

        void CircularArcItem::setStartAngle(qreal angle)
        {
            m_startAngle = angle;
        }

        qreal CircularArcItem::spanAngle() const
        {
            return m_spanAngle;
        }

        void CircularArcItem::setSpanAngle(qreal angle)
        {
            m_spanAngle = angle;
        }

        Item::Type CircularArcItem::type() const
        {
            return CircularArc;
        }

        Item *CircularArcItem::clone() const
        {
            auto item = new CircularArcItem;
            *item = *this;
            return item;
        }

        QString CircularArcItem::friendlyTypeName() const
        {
            return "Circuar Arc";
        }

        QString CircularArcItem::friendlyPropertyName(quint64 id) const
        {
            switch (id)
            {
                case RadiusProperty:
                    return "Radius";
                case StartAngleProperty:
                    return "Start angle";
                case SpanAngleProperty:
                    return "Span angle";
                default:
                    return Item::friendlyPropertyName(id);
            }
        }

        QIcon CircularArcItem::icon() const
        {
            return QIcon::fromTheme("draw-halfcircle3");
        }

        QVariant CircularArcItem::property(int id)
        {
            switch (id)
            {
                case RadiusProperty:
                    return m_radius;
                case StartAngleProperty:
                    return m_startAngle;
                case SpanAngleProperty:
                    return m_spanAngle;
                default:
                    return Item::property(id);
            }
        }

        void CircularArcItem::setProperty(int id, const QVariant &value)
        {
            switch (id)
            {
                case RadiusProperty:
                    m_radius = value.toReal();
                    break;
                case StartAngleProperty:
                    m_startAngle = value.toReal();
                    break;
                case SpanAngleProperty:
                    m_spanAngle = value.toReal();
                    break;
                default:
                    Item::setProperty(id, value);
                    break;
            }
        }

        EllipseItem::EllipseItem():
            m_xRadius(0.0),
            m_yRadius(0.0)
        {

        }

        EllipseItem::~EllipseItem()
        {

        }

        qreal EllipseItem::xRadius() const
        {
            return m_xRadius;
        }

        void EllipseItem::setXRadius(qreal radius)
        {
            m_xRadius = radius;
        }

        qreal EllipseItem::yRadius() const
        {
            return m_yRadius;
        }

        void EllipseItem::setYRadius(qreal radius)
        {
            m_yRadius = radius;
        }

        Item::Type EllipseItem::type() const
        {
            return Ellipse;
        }

        Item *EllipseItem::clone() const
        {
            auto item = new EllipseItem;
            *item = *this;
            return item;
        }

        QString EllipseItem::friendlyTypeName() const
        {
            return "Ellipse";
        }

        QString EllipseItem::friendlyPropertyName(quint64 id) const
        {
            switch (id)
            {
                case XRadiusProperty:
                    return "X Radius";
                case YRadiusProperty:
                    return "Y Radius";
                default:
                    return Item::friendlyPropertyName(id);
            }
        }

        QIcon EllipseItem::icon() const
        {
            return QIcon::fromTheme("draw-ellipse");
        }

        QVariant EllipseItem::property(int id)
        {
            switch (id)
            {
                case XRadiusProperty:
                    return m_xRadius;
                case YRadiusProperty:
                    return m_yRadius;
                default:
                    return Item::property(id);
            }
        }

        void EllipseItem::setProperty(int id, const QVariant &value)
        {
            switch (id)
            {
                case XRadiusProperty:
                    m_xRadius = value.toReal();
                    break;
                case YRadiusProperty:
                    m_yRadius = value.toReal();
                    break;
                default:
                    Item::setProperty(id, value);
                    break;
            }
        }

        EllipticalArcItem::EllipticalArcItem():
            m_xRadius(0.0),
            m_yRadius(0.0),
            m_startAngle(0.0),
            m_spanAngle(360.0)
        {

        }

        EllipticalArcItem::~EllipticalArcItem()
        {

        }

        qreal EllipticalArcItem::xRadius() const
        {
            return m_xRadius;
        }

        void EllipticalArcItem::setXRadius(qreal radius)
        {
            m_xRadius = radius;
        }

        qreal EllipticalArcItem::yRadius() const
        {
            return m_yRadius;
        }

        void EllipticalArcItem::setYRadius(qreal radius)
        {
            m_yRadius = radius;
        }

        qreal EllipticalArcItem::startAngle() const
        {
            return  m_startAngle;
        }

        void EllipticalArcItem::setStartAngle(qreal angle)
        {
            m_startAngle = angle;
        }

        qreal EllipticalArcItem::spanAngle() const
        {
            return m_spanAngle;
        }

        void EllipticalArcItem::setSpanAngle(qreal angle)
        {
            m_spanAngle = angle;
        }

        Item::Type EllipticalArcItem::type() const
        {
            return EllipticalArc;
        }

        Item *EllipticalArcItem::clone() const
        {
            auto item = new EllipticalArcItem;
            *item = *this;
            return item;
        }

        QString EllipticalArcItem::friendlyTypeName() const
        {
            return "Elliptical arc";
        }

        QString EllipticalArcItem::friendlyPropertyName(quint64 id) const
        {
            switch (id)
            {
                case XRadiusProperty:
                    return "X Radius";
                case YRadiusProperty:
                    return "Y Radius";
                case StartAngleProperty:
                    return "Start angle";
                case SpanAngleProperty:
                    return "Span angle";
                default:
                    return Item::friendlyPropertyName(id);
            }
        }

        QIcon EllipticalArcItem::icon() const
        {
            return QIcon::fromTheme("draw-halfcircle3");
        }

        QVariant EllipticalArcItem::property(int id)
        {
            switch (id)
            {
                case XRadiusProperty:
                    return m_xRadius;
                case YRadiusProperty:
                    return m_yRadius;
                case StartAngleProperty:
                    return m_startAngle;
                case SpanAngleProperty:
                    return m_spanAngle;
                default:
                    return Item::property(id);
            }
        }

        void EllipticalArcItem::setProperty(int id, const QVariant &value)
        {
            switch (id)
            {
                case XRadiusProperty:
                    m_xRadius = value.toReal();
                    break;
                case YRadiusProperty:
                    m_yRadius = value.toReal();
                    break;
                case StartAngleProperty:
                    m_startAngle = value.toReal();
                    break;
                case SpanAngleProperty:
                    m_spanAngle = value.toReal();
                    break;
                default:
                    Item::setProperty(id, value);
                    break;
            }
        }

        PolylineItem::PolylineItem()
        {

        }

        PolylineItem::~PolylineItem()
        {

        }

        QList<QPointF> PolylineItem::vertices() const
        {
            return m_vertices;
        }

        void PolylineItem::setVertices(const QList<QPointF> &vertices)
        {
            m_vertices = vertices;
        }

        Item::Type PolylineItem::type() const
        {
            return Polyline;
        }

        Item *PolylineItem::clone() const
        {
            auto item = new PolylineItem;
            *item = *this;
            return item;
        }

        QString PolylineItem::friendlyTypeName() const
        {
            return "Polyline";
        }

        QString PolylineItem::friendlyPropertyName(quint64 id) const
        {
            switch (id)
            {
                case VerticesProperty:
                    return "Vertices";
                default:
                    return Item::friendlyPropertyName(id);
            }
        }

        QIcon PolylineItem::icon() const
        {
            return QIcon::fromTheme("draw-line");
        }

        QVariant PolylineItem::property(int id)
        {
            switch (id)
            {
                case VerticesProperty:
                    return QVariant::fromValue(m_vertices);
                default:
                    return Item::property(id);
            }
        }

        void PolylineItem::setProperty(int id, const QVariant &value)
        {
            switch (id)
            {
                case VerticesProperty:
                {
                    QVariantList valueList = value.toList();
                    m_vertices.reserve(valueList.count());
                    for (const QVariant item: valueList)
                    {
                        m_vertices.append(item.toPointF());
                    }
                    break;
                }
                default:
                    Item::setProperty(id, value);
                    break;
            }
        }

        PolygonItem::PolygonItem()
        {

        }

        PolygonItem::~PolygonItem()
        {

        }

        QList<QPointF> PolygonItem::vertices() const
        {
            return m_vertices;
        }

        void PolygonItem::setVertices(const QList<QPointF> &vertices)
        {
            m_vertices = vertices;
        }

        Item::Type PolygonItem::type() const
        {
            return Polygon;
        }

        Item *PolygonItem::clone() const
        {
            auto item = new PolygonItem;
            *item = *this;
            return item;
        }

        QString PolygonItem::friendlyTypeName() const
        {
            return "Polygon";
        }

        QString PolygonItem::friendlyPropertyName(quint64 id) const
        {
            switch (id)
            {
                case VerticesProperty:
                    return "Vertices";
                default:
                    return Item::friendlyPropertyName(id);
            }
        }

        QIcon PolygonItem::icon() const
        {
            return QIcon::fromTheme("draw-polygon");
        }

        QVariant PolygonItem::property(int id)
        {
            switch (id)
            {
                case VerticesProperty:
                    return QVariant::fromValue(m_vertices);
                default:
                    return Item::property(id);
            }
        }

        void PolygonItem::setProperty(int id, const QVariant &value)
        {
            switch (id)
            {
                case VerticesProperty:
                {
                    QVariantList valueList = value.toList();
                    m_vertices.reserve(valueList.count());
                    for (const QVariant item: valueList)
                    {
                        m_vertices.append(item.toPointF());
                    }
                    break;
                }
                default:
                    Item::setProperty(id, value);
                    break;
            }
        }

        LabelItem::LabelItem():
            m_text(""),
            m_textColor("#000000"), // FIXME
            m_fontFamily(""), // FIXME
            m_fontSize(12) // FIXME
        {

        }

        LabelItem::~LabelItem()
        {

        }

        QString LabelItem::text() const
        {
            return m_text;
        }

        void LabelItem::setText(const QString &text)
        {
            m_text = text;
        }

        QString LabelItem::textColor() const
        {
            return m_textColor;
        }

        void LabelItem::setTextColor(const QString &color)
        {
            m_textColor = color;
        }

        QString LabelItem::fontFamily() const
        {
            return m_fontFamily;
        }

        void LabelItem::setFontFamily(const QString &family)
        {
            m_fontFamily = family;
        }

        int LabelItem::fontSize() const
        {
            return m_fontSize;
        }

        void LabelItem::setFontSize(int size)
        {
            m_fontSize = size;
        }

        Item::Type LabelItem::type() const
        {
            return Label;
        }

        Item *LabelItem::clone() const
        {
            auto item = new LabelItem;
            *item = *this;
            return item;
        }

        QString LabelItem::friendlyTypeName() const
        {
            return "Label";
        }

        QString LabelItem::friendlyPropertyName(quint64 id) const
        {
            switch (id)
            {
                case FontFamilyProperty:
                    return "Font family";
                case FontSizeProperty:
                    return "Font size";
                case TextProperty:
                    return "Text";
                case TextColorProperty:
                    return "Text color";
                default:
                    return Item::friendlyPropertyName(id);
            }
        }

        QIcon LabelItem::icon() const
        {
            return QIcon::fromTheme("insert-text");
        }

        QVariant LabelItem::property(int id)
        {
            switch (id)
            {
                case FontFamilyProperty:
                    return m_fontFamily;
                case FontSizeProperty:
                    return m_fontSize;
                case TextProperty:
                    return m_text;
                case TextColorProperty:
                    return m_textColor;
                default:
                    return Item::property(id);
            }
        }

        void LabelItem::setProperty(int id, const QVariant &value)
        {
            switch (id)
            {
                case FontFamilyProperty:
                    m_fontFamily = value.toString();
                    break;
                case FontSizeProperty:
                    m_fontSize = value.toInt();
                    break;
                case TextProperty:
                    m_text = value.toString();
                    break;
                case TextColorProperty:
                    m_textColor = value.toString();
                    break;
                default:
                    Item::setProperty(id, value);
                    break;
            }
        }

        PinItem::PinItem()
        {

        }

        PinItem::~PinItem()
        {
            delete(designator);
            delete(label);
        }

        Item::Type PinItem::type() const
        {
            return Pin;
        }

        // FIXME: have to deep clone
        Item *PinItem::clone() const
        {
            auto item = new PinItem;
            *item = *this;
            return item;
        }

        QString PinItem::friendlyTypeName() const
        {
            return "Pin";
        }

        QString PinItem::friendlyPropertyName(quint64 id) const
        {
            switch (id)
            {
                default:
                    return Item::friendlyPropertyName(id);
            }
        }

        QIcon PinItem::icon() const
        {
            return QIcon::fromTheme("network-connect");
        }

        QVariant PinItem::property(int id)
        {
            switch (id)
            {
                default:
                    return Item::property(id);
            }
        }

        void PinItem::setProperty(int id, const QVariant &value)
        {
            switch (id)
            {
                default:
                    Item::setProperty(id, value);
                    break;
            }
        }

        ItemGroup::ItemGroup()
        {

        }

        ItemGroup::~ItemGroup()
        {
            qDeleteAll(children);
        }

        Item::Type ItemGroup::type() const
        {
            return Group;
        }

        // FIXME: have to deep clone
        Item *ItemGroup::clone() const
        {
            auto item = new ItemGroup;
            *item = *this;
            return item;
        }

        QString ItemGroup::friendlyTypeName() const
        {
            return "Group";
        }

        QString ItemGroup::friendlyPropertyName(quint64 id) const
        {
            switch (id)
            {
                default:
                    return Item::friendlyPropertyName(id);
            }
        }

        QIcon ItemGroup::icon() const
        {
            return QIcon::fromTheme("object-group");
        }

        QVariant ItemGroup::property(int id)
        {
            switch (id)
            {
                default:
                    return Item::property(id);
            }
        }

        void ItemGroup::setProperty(int id, const QVariant &value)
        {
            switch (id)
            {
                default:
                    Item::setProperty(id, value);
                    break;
            }
        }

    }
}
