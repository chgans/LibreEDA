#include "xdl/symboldocumentwriter.h"
#include "xdl/symbolitem.h"
#include "xdl/symboldocument.h"

#include <QFile>
#include <QXmlStreamWriter>
#include <QPen>
#include <QBrush>

namespace xdl { namespace symbol {


struct DocumentWriterPrivate
{
    // Polymorphics
    void writeGraphicsItem(Item *item);
    void writeItemList(const char *listTag, QList<Item *> items);

    // Items
    void writePolyline(PolylineItem *item);
    void writePolygon(PolygonItem *item);
    void writeRectangle(RectangleItem *item);
    void writeCircle(CircleItem *item);
    void writeCircularArc(CircularArcItem *item);
    void writeEllipse(EllipseItem *item);
    void writeEllipticalArc(EllipticalArcItem *item);
    void writeLabel(LabelItem *item);
    void writePin(PinItem *item);
    void writeGroup(ItemGroup *item);

    // Abstract items
    void writeItem(Item *item);

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

    QXmlStreamWriter *writer;
};

DocumentWriter::DocumentWriter()
{
    p->writer = new QXmlStreamWriter();
}

bool DocumentWriter::write(const QString &filename, const Document *symbol)
{
    QFile file(filename);
    if (!file.open(QFile::WriteOnly | QIODevice::Truncate)) {
        m_errorString = QString("\"%1\": %2").arg(filename).arg(file.errorString());
        return false;
    }
    p->writer->setDevice(&file);
    p->writer->setAutoFormatting(true);
    p->writer->writeStartDocument("1.0");
    p->writer->writeDefaultNamespace("http://www.leda.org/xsd");
    p->writer->writeStartElement("symbol");
    p->writer->writeTextElement("name", symbol->symbolName());
    p->writer->writeTextElement("label", symbol->symbolDescription());
    //p->writeItemList("drawing", symbol->it);
    p->writer->writeEndElement(); // symbol
    p->writer->writeEndDocument();
    return true;
}



void DocumentWriterPrivate::writeGraphicsItem(Item *item)
{
//    QString type = item->data(0).toString();
//    writer->writeStartElement(type);
//    if (type == "ellipse") {
//        writeEllipse(item);
//    }
//    else if (type == "line") {
//        writePolyline(item);
//    }
//    else if (type == "pin") {
//        writePin(item);
//    }
//    else if (type == "rectangle") {
//        writeRectangle(item);
//    }
//    else if (type == "group") {
//        writeGroup(item);
//    }
//    else if (type == "arc") {
//        writeArc(item);
//    }
//    else if (type == "label") {
//        writeLabel(item);
//    }
//    writer->writeEndElement();
}

void DocumentWriterPrivate::writeItemList(const char *listTag, QList<Item *> items)
{
    writer->writeStartElement(listTag);
//    foreach (QGraphicsItem *item, items) {
//       writeGraphicsItem(item);
//    }
    writer->writeEndElement();
}

void DocumentWriterPrivate::writePolyline(PolylineItem *item)
{
    writeItem(item);
    writePointList("vertices", "point", item->vertices);
    writer->writeEndElement();
}

void DocumentWriterPrivate::writePolygon(PolygonItem *item)
{
    writeItem(item);
    writePointList("vertices", "point", item->vertices);
    writer->writeEndElement();
}

void DocumentWriterPrivate::writeRectangle(RectangleItem *item)
{
    writeItem(item);
    writePoint("top-left", item->topLeft);
    writePoint("bottom-right", item->bottomRight);
}

void DocumentWriterPrivate::writeCircle(CircleItem *item)
{
    writeItem(item);
    writePoint("center", item->center);
    writeDouble("radius", item->radius);
}

void DocumentWriterPrivate::writeCircularArc(CircularArcItem *item)
{
    writeItem(item);
    writePoint("center", item->center);
    writeDouble("radius", item->radius);
    writeDouble("start-angle", item->startAngle);
    writeDouble("span-angle", item->spanAngle);
}

void DocumentWriterPrivate::writeEllipse(EllipseItem *item)
{
    writeItem(item);
    writePoint("center", item->center);
    writeDouble("x-radius", item->xRadius);
    writeDouble("y-radius", item->yRadius);
}

void DocumentWriterPrivate::writeEllipticalArc(EllipticalArcItem *item)
{
    writeItem(item);
    writePoint("center", item->center);
    writeDouble("x-radius", item->xRadius);
    writeDouble("y-radius", item->yRadius);
    writeDouble("start-angle", item->startAngle);
    writeDouble("span-angle", item->spanAngle);
}

void DocumentWriterPrivate::writeLabel(LabelItem *item)
{
    writeItem(item);
    // FIXME
}

void DocumentWriterPrivate::writePin(PinItem *item)
{
    writeItem(item);
    writer->writeTextElement("designator", item->designator.text); // FIXME
    writer->writeTextElement("label", item->label.text); // FIXME
}

void DocumentWriterPrivate::writeGroup(ItemGroup *item)
{
    writeItem(item);
    // writeItemList("children", item->childrenId);
}

void DocumentWriterPrivate::writeItem(Item *item)
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

void DocumentWriterPrivate::writePen(const char *tag, const QPen &pen)
{
    writer->writeStartElement(tag);
    writeDouble("width", pen.width());
    writeColor("color", pen.color());
    writePenStyle("style", pen.style());
    writePenCapStyle("cap-style", pen.capStyle());
    writePenJoinStyle("join-style", pen.joinStyle());
    writer->writeEndElement();
}

void DocumentWriterPrivate::writeBrush(const char *tag, const QBrush &brush)
{
    writer->writeStartElement(tag);
    writeColor("color", brush.color());
    writeBrushStyle("style", brush.style());
    writer->writeEndElement();
}

void DocumentWriterPrivate::writePoint(const char *tag, const QPointF &point)
{
    writer->writeStartElement(tag);
    writeDouble("x", point.x());
    writeDouble("y", point.y());
    writer->writeEndElement();
}

void DocumentWriterPrivate::writePointList(const char *listTag, const char *pointTag, const QList<QPointF> &pointList)
{
    writer->writeStartElement(listTag);
    foreach (const QPointF &pos, pointList)
        writePoint(pointTag, pos);
    writer->writeEndElement();
}

void DocumentWriterPrivate::writeColor(const char *tag, const QColor &color)
{
    writer->writeTextElement(tag, color.name());
}

void DocumentWriterPrivate::writePenStyle(const char *tag, Qt::PenStyle style)
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
    writer->writeTextElement(tag, str);
}

void DocumentWriterPrivate::writePenCapStyle(const char *tag, Qt::PenCapStyle style)
{
    QString str;
    switch (style) {
    case Qt::FlatCap: str = "Flat"; break;
    case Qt::SquareCap: str = "Square"; break;
    default: str = "Round";
    }
    writer->writeTextElement(tag, str);
}

void DocumentWriterPrivate::writePenJoinStyle(const char *tag, Qt::PenJoinStyle style)
{
    QString str;
    switch (style) {
    case Qt::MiterJoin: str = "Miter"; break;
    case Qt::BevelJoin: str = "Bevel"; break;
    default: str = "Round";
    }
    writer->writeTextElement(tag, str);
}

void DocumentWriterPrivate::writeBrushStyle(const char *tag, Qt::BrushStyle style)
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
    writer->writeTextElement(tag, str);
}

void DocumentWriterPrivate::writeBoolean(const char *tag, bool value)
{
    writer->writeTextElement(tag, value ? "true" : "false");
}

void DocumentWriterPrivate::writeInt(const char *tag, int value)
{
    writer->writeTextElement(tag, QString("%1").arg(value));
}

void DocumentWriterPrivate::writeDouble(const char *tag, qreal value)
{
    writer->writeTextElement(tag, QString("%1").arg(value, 0, 'E', 6));
}

}}
