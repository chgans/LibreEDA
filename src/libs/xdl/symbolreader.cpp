#include "symbolreader.h"
#include "leda-xdl-symbol-pimpl.h"

#include <sstream>

namespace xdl {
namespace symbol {

struct ReaderPrivate
{
    ReaderPrivate()
    {
        // Connect the parsers together.
        //
        Symbol_p.parsers(string_p,
                         string_p,
                         ItemList_p);

        ItemList_p.parsers(Polyline_p,
                           Polygon_p,
                           Rectangle_p,
                           Circle_p,
                           CircularArc_p,
                           Ellipse_p,
                           EllipticalArc_p,
                           Label_p,
                           Pin_p,
                           ItemGroup_p);


        Polyline_p.parsers(Point_p,
                           LineStyle_p,
                           LineWidth_p,
                           Color_p,
                           FillStyle_p,
                           Color_p,
                           double_p,
                           Angle_p,
                           Opacity_p,
                           boolean_p,
                           boolean_p,
                           boolean_p,
                           boolean_p,
                           PointList_p);

        Polygon_p.parsers(Point_p,
                          LineStyle_p,
                          LineWidth_p,
                          Color_p,
                          FillStyle_p,
                          Color_p,
                          double_p,
                          Angle_p,
                          Opacity_p,
                          boolean_p,
                          boolean_p,
                          boolean_p,
                          boolean_p,
                          PointList_p);

        Rectangle_p.parsers(Point_p,
                            LineStyle_p,
                            LineWidth_p,
                            Color_p,
                            FillStyle_p,
                            Color_p,
                            double_p,
                            Angle_p,
                            Opacity_p,
                            boolean_p,
                            boolean_p,
                            boolean_p,
                            boolean_p,
                            NonNegativeDouble_p,
                            NonNegativeDouble_p);
        Circle_p.parsers(Point_p,
                         LineStyle_p,
                         LineWidth_p,
                         Color_p,
                         FillStyle_p,
                         Color_p,
                         double_p,
                         Angle_p,
                         Opacity_p,
                         boolean_p,
                         boolean_p,
                         boolean_p,
                         boolean_p,
                         NonNegativeDouble_p);

        CircularArc_p.parsers(Point_p,
                              LineStyle_p,
                              LineWidth_p,
                              Color_p,
                              FillStyle_p,
                              Color_p,
                              double_p,
                              Angle_p,
                              Opacity_p,
                              boolean_p,
                              boolean_p,
                              boolean_p,
                              boolean_p,
                              NonNegativeDouble_p,
                              Angle_p,
                              Angle_p);

        Ellipse_p.parsers(Point_p,
                          LineStyle_p,
                          LineWidth_p,
                          Color_p,
                          FillStyle_p,
                          Color_p,
                          double_p,
                          Angle_p,
                          Opacity_p,
                          boolean_p,
                          boolean_p,
                          boolean_p,
                          boolean_p,
                          NonNegativeDouble_p,
                          NonNegativeDouble_p);

        EllipticalArc_p.parsers(Point_p,
                                LineStyle_p,
                                LineWidth_p,
                                Color_p,
                                FillStyle_p,
                                Color_p,
                                double_p,
                                Angle_p,
                                Opacity_p,
                                boolean_p,
                                boolean_p,
                                boolean_p,
                                boolean_p,
                                NonNegativeDouble_p,
                                NonNegativeDouble_p,
                                Angle_p,
                                Angle_p);

        Label_p.parsers(Point_p,
                        LineStyle_p,
                        LineWidth_p,
                        Color_p,
                        FillStyle_p,
                        Color_p,
                        double_p,
                        Angle_p,
                        Opacity_p,
                        boolean_p,
                        boolean_p,
                        boolean_p,
                        boolean_p,
                        string_p,
                        Color_p,
                        non_negative_integer_p);

        Pin_p.parsers(Point_p,
                      LineStyle_p,
                      LineWidth_p,
                      Color_p,
                      FillStyle_p,
                      Color_p,
                      double_p,
                      Angle_p,
                      Opacity_p,
                      boolean_p,
                      boolean_p,
                      boolean_p,
                      boolean_p,
                      string_p,
                      string_p);

        ItemGroup_p.parsers(Point_p,
                            LineStyle_p,
                            LineWidth_p,
                            Color_p,
                            FillStyle_p,
                            Color_p,
                            double_p,
                            Angle_p,
                            Opacity_p,
                            boolean_p,
                            boolean_p,
                            boolean_p,
                            boolean_p,
                            ItemList_p);


        Point_p.parsers(double_p,
                        double_p);

        PointList_p.parsers(Point_p);
    }

    // Instantiate individual parsers.
    //
    Symbol_pimpl Symbol_p;
    ::xml_schema::string_pimpl string_p;
    ItemList_pimpl ItemList_p;
    Point_pimpl Point_p;
    ::xml_schema::double_pimpl double_p;
    Angle_pimpl Angle_p;
    Opacity_pimpl Opacity_p;
    ::xml_schema::boolean_pimpl boolean_p;
    NonNegativeDouble_pimpl NonNegativeDouble_p;
    Color_pimpl Color_p;
    LineStyle_pimpl LineStyle_p;
    LineWidth_pimpl LineWidth_p;
    FillStyle_pimpl FillStyle_p;
    Polyline_pimpl Polyline_p;
    Polygon_pimpl Polygon_p;
    PointList_pimpl PointList_p;
    Pin_pimpl Pin_p;
    Rectangle_pimpl Rectangle_p;
    Circle_pimpl Circle_p;
    CircularArc_pimpl CircularArc_p;
    Ellipse_pimpl Ellipse_p;
    EllipticalArc_pimpl EllipticalArc_p;
    ItemGroup_pimpl ItemGroup_p;
    Label_pimpl Label_p;
    ::xml_schema::non_negative_integer_pimpl non_negative_integer_p;
};

Reader::Reader():
    p(new ReaderPrivate)
{
}

Symbol *Reader::read(const QString &filename)
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
    catch (const std::ios_base::failure &)
    {
        m_errorString = QString("General I/O failure");
        return nullptr;
    }
}

QString Reader::errorString() const
{
    return m_errorString;
}

}
}
