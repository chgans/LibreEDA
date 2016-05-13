#include "xdl/symbolwriter.h"
#include "xdl/symbol.h"

#include <QFile>
#include <QXmlStreamWriter>
#include <QPen>
#include <QBrush>

namespace xdl { namespace symbol {


struct WriterPrivate
{

    // Polymorphics
    void writeItem(const Item *item);
    void writeItemList(const char *listTag, const QList<Item *> &items);

    // Symbol and Items
    void writeSymbol(const Symbol *symbol);
    void writePolyline(const PolylineItem *item);
    void writePolygon(const PolygonItem *item);
    void writeRectangle(const RectangleItem *item);
    void writeCircle(const CircleItem *item);
    void writeCircularArc(const CircularArcItem *item);
    void writeEllipse(const EllipseItem *item);
    void writeEllipticalArc(const EllipticalArcItem *item);
    void writeLabel(const LabelItem *item);
    void writePin(const PinItem *item);
    void writeGroup(const ItemGroup *item);

    // Abstract items
    void writeItemContent(const Item *item);

    // Complex types
    void writePen(const char *tag, const QPen &pen);
    void writeBrush(const char *tag, const QBrush &brush);
    void writePoint(const char *tag, const QPointF &point);
    void writePointList(const char *listTag, const char *pointTag, const QList<QPointF> &pointList);

    // Simple types
    void writeColor(const char *tag, const QColor &color);
    void writePenStyle(const char *tag, Qt::PenStyle style);
    void writePenCapStyle(const char *tag, Qt::PenCapStyle style);
    void writePenJoinStyle(const char *tag, Qt::PenJoinStyle style);
    void writeBrushStyle(const char *tag, Qt::BrushStyle style);

    // Built in types
    void writeBoolean(const char *tag, bool value);
    void writeInt(const char *tag, int value);
    void writeDouble(const char *tag, qreal value);

    QXmlStreamWriter *xmlWriter;
};

Writer::Writer():
    p(new WriterPrivate)
{
    p->xmlWriter = new QXmlStreamWriter();
}

Writer::~Writer()
{
    delete p;
}

bool Writer::write(const QString &filename, const Symbol *symbol)
{
    QFile file(filename);
    if (!file.open(QFile::WriteOnly | QIODevice::Truncate)) {
        m_errorString = QString("\"%1\": %2").arg(filename).arg(file.errorString());
        return false;
    }
    p->xmlWriter->setDevice(&file);
    p->xmlWriter->setAutoFormatting(true);
    p->xmlWriter->writeStartDocument("1.0");
    p->xmlWriter->writeDefaultNamespace("http://www.leda.org/xdl");
    p->writeSymbol(symbol);
    p->xmlWriter->writeEndDocument();
    return true;
}

QString Writer::errorString() const
{
    return m_errorString;
}

void WriterPrivate::writeSymbol(const Symbol *symbol)
{
    xmlWriter->writeStartElement("symbol");
    xmlWriter->writeTextElement("name", symbol->name);
    xmlWriter->writeTextElement("label", symbol->description);
    writeItemList("drawing", symbol->drawingItems);
    xmlWriter->writeEndElement();
}

void WriterPrivate::writeItem(const Item *item)
{
    switch (item->type())
    {
    case Item::Rectangle:
        writeRectangle(reinterpret_cast<const RectangleItem*>(item));
        break;
    case Item::Circle:
        writeCircle(reinterpret_cast<const CircleItem*>(item));
        break;
    case Item::CircularArc:
        writeCircularArc(reinterpret_cast<const CircularArcItem*>(item));
        break;
    case Item::Ellipse:
        writeEllipse(reinterpret_cast<const EllipseItem*>(item));
        break;
    case Item::EllipticalArc:
        writeEllipticalArc(reinterpret_cast<const EllipticalArcItem*>(item));
        break;
    case Item::Polyline:
        writePolyline(reinterpret_cast<const PolylineItem*>(item));
        break;
    case Item::Polygon:
        writePolygon(reinterpret_cast<const PolygonItem*>(item));
        break;
    case Item::Label:
        writeLabel(reinterpret_cast<const LabelItem*>(item));
        break;
    case Item::Pin:
        writePin(reinterpret_cast<const PinItem*>(item));
        break;
    case Item::Group:
        writeGroup(reinterpret_cast<const ItemGroup*>(item));
        break;
    }
}

void WriterPrivate::writeItemList(const char *listTag, const QList<Item *> &items)
{
    xmlWriter->writeStartElement(listTag);
    for (Item *item: items) {
       writeItem(item);
    }
    xmlWriter->writeEndElement();
}

void WriterPrivate::writePolyline(const PolylineItem *item)
{
    xmlWriter->writeStartElement("polyline");
    writeItemContent(item);
    writePointList("vertices", "point", item->vertices);
    xmlWriter->writeEndElement();
}

void WriterPrivate::writePolygon(const PolygonItem *item)
{
    xmlWriter->writeStartElement("polygon");
    writeItemContent(item);
    writePointList("vertices", "point", item->vertices);
    xmlWriter->writeEndElement();
}

void WriterPrivate::writeRectangle(const RectangleItem *item)
{
    xmlWriter->writeStartElement("rectangle");
    writeItemContent(item);
    writePoint("top-left", item->topLeft);
    writePoint("bottom-right", item->bottomRight);
    xmlWriter->writeEndElement();
}

void WriterPrivate::writeCircle(const CircleItem *item)
{
    xmlWriter->writeStartElement("circle");
    writeItemContent(item);
    writePoint("center", item->center);
    writeDouble("radius", item->radius);
    xmlWriter->writeEndElement();
}

void WriterPrivate::writeCircularArc(const CircularArcItem *item)
{
    xmlWriter->writeStartElement("circular-arc");
    writeItemContent(item);
    writePoint("center", item->center);
    writeDouble("radius", item->radius);
    writeDouble("start-angle", item->startAngle);
    writeDouble("span-angle", item->spanAngle);
    xmlWriter->writeEndElement();
}

void WriterPrivate::writeEllipse(const EllipseItem *item)
{
    xmlWriter->writeStartElement("ellipse");
    writeItemContent(item);
    writePoint("center", item->center);
    writeDouble("x-radius", item->xRadius);
    writeDouble("y-radius", item->yRadius);
    xmlWriter->writeEndElement();
}

void WriterPrivate::writeEllipticalArc(const EllipticalArcItem *item)
{
    xmlWriter->writeStartElement("elliptical-arc");
    writeItemContent(item);
    writePoint("center", item->center);
    writeDouble("x-radius", item->xRadius);
    writeDouble("y-radius", item->yRadius);
    writeDouble("start-angle", item->startAngle);
    writeDouble("span-angle", item->spanAngle);
    xmlWriter->writeEndElement();
}

void WriterPrivate::writeLabel(const LabelItem *item)
{
    xmlWriter->writeStartElement("label");
    writeItemContent(item);
    // FIXME
    xmlWriter->writeEndElement();
}

void WriterPrivate::writePin(const PinItem *item)
{
    xmlWriter->writeStartElement("pin");
    writeItemContent(item);
    xmlWriter->writeTextElement("designator", item->designator->text); // FIXME
    xmlWriter->writeTextElement("label", item->label->text); // FIXME
    xmlWriter->writeEndElement();
}

void WriterPrivate::writeGroup(const ItemGroup *item)
{
    xmlWriter->writeStartElement("group");
    writeItemContent(item);
    // writeItemList("children", item->childrenId);
    xmlWriter->writeEndElement();
}

void WriterPrivate::writeItemContent(const Item *item)
{
    writePen("pen", item->pen);
    writeBrush("brush", item->brush);
    writePoint("position", item->position);
    writeDouble("z-value", item->zValue);
    writeDouble("rotation", item->rotation); // % 360
    writeDouble("opacity", item->opacity);
    writeBoolean("locked", !item->locked);
    writeBoolean("x-mirrored", item->xMirrored);
    writeBoolean("y-mirrored", item->yMirrored);
    writeBoolean("visible", item->visible);
}

void WriterPrivate::writePen(const char *tag, const QPen &pen)
{
    xmlWriter->writeStartElement(tag);
    writeDouble("width", pen.width());
    writeColor("color", pen.color());
    writePenStyle("style", pen.style());
    writePenCapStyle("cap-style", pen.capStyle());
    writePenJoinStyle("join-style", pen.joinStyle());
    xmlWriter->writeEndElement();
}

void WriterPrivate::writeBrush(const char *tag, const QBrush &brush)
{
    xmlWriter->writeStartElement(tag);
    writeColor("color", brush.color());
    writeBrushStyle("style", brush.style());
    xmlWriter->writeEndElement();
}

void WriterPrivate::writePoint(const char *tag, const QPointF &point)
{
    xmlWriter->writeStartElement(tag);
    writeDouble("x", point.x());
    writeDouble("y", point.y());
    xmlWriter->writeEndElement();
}

void WriterPrivate::writePointList(const char *listTag, const char *pointTag, const QList<QPointF> &pointList)
{
    xmlWriter->writeStartElement(listTag);
    for (const QPointF &pos: pointList)
        writePoint(pointTag, pos);
    xmlWriter->writeEndElement();
}

void WriterPrivate::writeColor(const char *tag, const QColor &color)
{
    xmlWriter->writeTextElement(tag, color.name());
}

void WriterPrivate::writePenStyle(const char *tag, Qt::PenStyle style)
{
    QString str;
    switch (style) {
        case Qt::DashLine: str = "DashLine"; break;
        case Qt::DotLine: str = "DotLine"; break;
        case Qt::DashDotLine: str = "DashDotLine"; break;
        case Qt::DashDotDotLine: str = "DashDotDotLine"; break;
        case Qt::NoPen: str = "NoPen"; break;
        default: str = "SolidLine";
    }
    xmlWriter->writeTextElement(tag, str);
}

void WriterPrivate::writePenCapStyle(const char *tag, Qt::PenCapStyle style)
{
    QString str;
    switch (style) {
    case Qt::FlatCap: str = "Flat"; break;
    case Qt::SquareCap: str = "Square"; break;
    default: str = "Round";
    }
    xmlWriter->writeTextElement(tag, str);
}

void WriterPrivate::writePenJoinStyle(const char *tag, Qt::PenJoinStyle style)
{
    QString str;
    switch (style) {
    case Qt::MiterJoin: str = "Miter"; break;
    case Qt::BevelJoin: str = "Bevel"; break;
    default: str = "Round";
    }
    xmlWriter->writeTextElement(tag, str);
}

void WriterPrivate::writeBrushStyle(const char *tag, Qt::BrushStyle style)
{
    QString str;
    switch (style) {
    case Qt::SolidPattern: str = "Solid"; break;
    case Qt::HorPattern: str = "Horizontal"; break;
    case Qt::VerPattern: str = "Vertical"; break;
    case Qt::CrossPattern: str = "Cross"; break;
    case Qt::BDiagPattern: str = "BDiagonal"; break;
    case Qt::FDiagPattern: str = "FDiagonal"; break;
    case Qt::DiagCrossPattern: str = "CrossDiagonal"; break;
    default: str = "NoBrush";
    }
    xmlWriter->writeTextElement(tag, str);
}

void WriterPrivate::writeBoolean(const char *tag, bool value)
{
    xmlWriter->writeTextElement(tag, value ? "true" : "false");
}

void WriterPrivate::writeInt(const char *tag, int value)
{
    xmlWriter->writeTextElement(tag, QString("%1").arg(value));
}

void WriterPrivate::writeDouble(const char *tag, qreal value)
{
    xmlWriter->writeTextElement(tag, QString("%1").arg(value, 0, 'E', 6));
}

}}
