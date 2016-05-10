#include "symboldocumentreader.h"
#include "leda-xdl-symbol-pimpl.h"

#include <sstream>

namespace xdl { namespace symbol {

struct SymbolReaderPrivate
{
    SymbolReaderPrivate()
    {
        // Connect the parsers together.
        //
        Symbol_p.parsers (string_p,
                          string_p,
                          ItemList_p);

        ItemList_p.parsers (Polyline_p,
                            Polygon_p,
                            Rectangle_p,
                            Circle_p,
                            CircularArc_p,
                            Ellipse_p,
                            EllipticalArc_p,
                            Label_p,
                            Pin_p,
                            ItemGroup_p);


        Polyline_p.parsers (Pen_p,
                            Brush_p,
                            Point_p,
                            double_p,
                            Angle_p,
                            Opacity_p,
                            boolean_p,
                            boolean_p,
                            boolean_p,
                            PointList_p);

        Polygon_p.parsers (Pen_p,
                           Brush_p,
                           Point_p,
                           double_p,
                           Angle_p,
                           Opacity_p,
                           boolean_p,
                           boolean_p,
                           boolean_p,
                           PointList_p);

        Rectangle_p.parsers (Pen_p,
                             Brush_p,
                             Point_p,
                             double_p,
                             Angle_p,
                             Opacity_p,
                             boolean_p,
                             boolean_p,
                             boolean_p,
                             Point_p,
                             Point_p);
        Circle_p.parsers(Pen_p,
                         Brush_p,
                         Point_p,
                         double_p,
                         Angle_p,
                         Opacity_p,
                         boolean_p,
                         boolean_p,
                         boolean_p,
                         Point_p,
                         NonNegativeDouble_p);

        CircularArc_p.parsers(Pen_p,
                              Brush_p,
                              Point_p,
                              double_p,
                              Angle_p,
                              Opacity_p,
                              boolean_p,
                              boolean_p,
                              boolean_p,
                              Point_p,
                              NonNegativeDouble_p,
                              Angle_p,
                              Angle_p);

        Ellipse_p.parsers (Pen_p,
                           Brush_p,
                           Point_p,
                           double_p,
                           Angle_p,
                           Opacity_p,
                           boolean_p,
                           boolean_p,
                           boolean_p,
                           Point_p,
                           NonNegativeDouble_p,
                           NonNegativeDouble_p);

        EllipticalArc_p.parsers (Pen_p,
                                 Brush_p,
                                 Point_p,
                                 double_p,
                                 Angle_p,
                                 Opacity_p,
                                 boolean_p,
                                 boolean_p,
                                 boolean_p,
                                 Point_p,
                                 NonNegativeDouble_p,
                                 NonNegativeDouble_p,
                                 Angle_p,
                                 Angle_p);

        Label_p.parsers (Pen_p,
                         Brush_p,
                         Point_p,
                         double_p,
                         Angle_p,
                         Opacity_p,
                         boolean_p,
                         boolean_p,
                         boolean_p,
                         string_p,
                         Font_p);

        Pin_p.parsers (Pen_p,
                       Brush_p,
                       Point_p,
                       double_p,
                       Angle_p,
                       Opacity_p,
                       boolean_p,
                       boolean_p,
                       boolean_p,
                       string_p,
                       string_p);

        ItemGroup_p.parsers (Pen_p,
                             Brush_p,
                             Point_p,
                             double_p,
                             Angle_p,
                             Opacity_p,
                             boolean_p,
                             boolean_p,
                             boolean_p,
                             ItemList_p);


        Point_p.parsers (double_p,
                         double_p);

        Pen_p.parsers (NonNegativeDouble_p,
                       Color_p,
                       PenStyle_p,
                       PenCapStyle_p,
                       PenJoinStyle_p);

        Brush_p.parsers (Color_p,
                         BrushStyle_p);

        PointList_p.parsers (Point_p);

        Font_p.parsers (string_p,
                        non_negative_integer_p,
                        boolean_p,
                        boolean_p,
                        boolean_p,
                        boolean_p);
    }

    // Instantiate individual parsers.
    //
    ::xdl::Symbol_pimpl Symbol_p;
    ::xml_schema::string_pimpl string_p;
    ::xdl::ItemList_pimpl ItemList_p;
    ::xdl::Point_pimpl Point_p;
    ::xml_schema::double_pimpl double_p;
    ::xdl::Angle_pimpl Angle_p;
    ::xdl::Opacity_pimpl Opacity_p;
    ::xml_schema::boolean_pimpl boolean_p;
    ::xdl::Pen_pimpl Pen_p;
    ::xdl::NonNegativeDouble_pimpl NonNegativeDouble_p;
    ::xdl::Color_pimpl Color_p;
    ::xdl::PenStyle_pimpl PenStyle_p;
    ::xdl::PenCapStyle_pimpl PenCapStyle_p;
    ::xdl::PenJoinStyle_pimpl PenJoinStyle_p;
    ::xdl::Brush_pimpl Brush_p;
    ::xdl::BrushStyle_pimpl BrushStyle_p;
    ::xdl::Polyline_pimpl Polyline_p;
    ::xdl::Polygon_pimpl Polygon_p;
    ::xdl::PointList_pimpl PointList_p;
    ::xdl::Pin_pimpl Pin_p;
    ::xdl::Rectangle_pimpl Rectangle_p;
    ::xdl::Circle_pimpl Circle_p;
    ::xdl::CircularArc_pimpl CircularArc_p;
    ::xdl::Ellipse_pimpl Ellipse_p;
    ::xdl::EllipticalArc_pimpl EllipticalArc_p;
    ::xdl::ItemGroup_pimpl ItemGroup_p;
    ::xdl::Label_pimpl Label_p;
    ::xdl::Font_pimpl Font_p;
    ::xml_schema::non_negative_integer_pimpl non_negative_integer_p;
};

SymbolReader::SymbolReader():
    p(new SymbolReaderPrivate)
{
}

Document *SymbolReader::parse(const QString &filename)
{
    m_errorString.clear();
    try
    {
        p->Symbol_p.pre();
        ::xml_schema::document doc_p(p->Symbol_p,
                                     "http://www.leda.org/xdl",
                                     "symbol");
        doc_p.parse(filename.toLocal8Bit().constData());
        return p->Symbol_p.post_Symbol();
    }
    catch (const ::xml_schema::parsing &e)
    {
        std::stringstream stream;
        stream << e;
        m_errorString = QString::fromStdString(stream.str());
        return nullptr;
    }
    catch (const std::ios_base::failure&)
    {
        m_errorString = QString("General I/O failure");
        return nullptr;
    }
}

}}
