#include "json.h"

#include <QPen>
#include <QBrush>

namespace Json
{

bool toPoint(QString *errorString, const QJsonValue &jsonValue, QPointF &value)
{
    QJsonArray array = jsonValue.toArray(); // returns an empty array is jsonValue is not an array
    if (array.count() != 2) {
        *errorString = "JSON value is not a point";
        return false;
    }
    qreal coords[2];
    for (int i =0; i < 2; i++) {
        if (!array.at(i).isDouble()) {
            *errorString = "JSON point is malformed";
            return false;
        }
        coords[i] = array.at(i).toDouble();
    }
    value.setX(coords[0]);
    value.setY(coords[1]);
    return true;
}

bool toPointList(QString *errorString, const QJsonValue &jsonValue, QList<QPointF> &value)
{
    if (!jsonValue.isArray()) {
        *errorString = "JSON value is not a list (of point)";
        return false;
    }
    QJsonArray array = jsonValue.toArray();
    QList<QPointF> list;
    for (int i = 0; i < array.count(); i++) {
        QPointF point;
        if (!toPoint(errorString, array.at(i), point))
            return false;
        list.append(point);
    }
    value = list;
    return true;
}

bool toRect(QString *errorString, const QJsonValue &jsonValue, QRectF &value)
{
    QList<QPointF> points;
    if (!toPointList(errorString, jsonValue, points))
        return false;
    if (!points.count() == 2) {
        *errorString = "JSON value is not a rectangle";
        return false;
    }
    value.setTopLeft(points[0]);
    value.setBottomRight(points[1]);
    return true;
}

bool toLine(QString *errorString, const QJsonValue &jsonValue, QLineF &value)
{
    QList<QPointF> points;
    if (!toPointList(errorString, jsonValue, points))
        return false;
    if (!points.count() == 2) {
        *errorString = "JSON value is not a line";
        return false;
    }
    value.setP1(points[0]);
    value.setP2(points[1]);
    return true;
}

bool toSize(QString *errorString, const QJsonValue &jsonValue, QSizeF &value)
{
    QPointF point;
    if (!toPoint(errorString, jsonValue, point))
        return false;
    value.setWidth(point.x());
    value.setHeight(point.y());
    return true;
}

bool toInt(QString *errorString, const QJsonValue &jsonValue, int &value)
{
    if (!jsonValue.isDouble()) {
        *errorString = "JSON value is not an integer number";
        return false;
    }
    value = jsonValue.toInt();
    return true;
}

bool toIntList(QString *errorString, const QJsonValue &jsonValue, QList<int> &value)
{
    if (!jsonValue.isArray()) {
        *errorString = "JSON value is not a list (of integer number)";
        return false;
    }
    QJsonArray array = jsonValue.toArray();
    QList<int> list;
    for (int i = 0; i < array.count(); i++) {
        int number;
        if (!toInt(errorString, array.at(i), number))
            return false;
        list.append(number);
    }
    value = list;
    return true;
}

bool toBool(QString *errorString, const QJsonValue &jsonValue, bool &value)
{
    if (!jsonValue.isBool()) {
        *errorString = "JSON value is not a boolean";
        return false;
    }
    value = jsonValue.toBool();
    return true;
}

bool toReal(QString *errorString, const QJsonValue &jsonValue, qreal &value)
{
    if (!jsonValue.isDouble()) {
        *errorString = "JSON value is not a real number";
        return false;
    }
    value = jsonValue.toDouble();
    return true;
}

bool toColor(QString *errorString, const QJsonValue &jsonValue, QColor &value)
{
    if (!jsonValue.isString()) {
        *errorString = "JSON value is not a color string";
        return false;
    }
    QString name = jsonValue.toString();
    if (!QColor::isValidColor(name)) {
        *errorString = "JSON value is not a valid color string";
        return false;
    }
    value.setNamedColor(name);
    return true;
}

bool toPen(QString *errorString, const QJsonValue &jsonValue, QPen &value)
{
    if (!jsonValue.isObject()) {
        *errorString = "JSON value is not a pen object";
        return false;
    }
    QJsonObject jsonObject = jsonValue.toObject();
    qreal width;
    if (!jsonObject.contains("width") || !toReal(errorString, jsonObject.value("width"), width)) {
        return false;
    }
    QColor color;
    if (!jsonObject.contains("color") || !toColor(errorString, jsonObject.value("color"), color)) {
        return false;
    }
    value.setWidthF(width);
    value.setColor(color);
    return true;
}

bool toBrush(QString *errorString, const QJsonValue &jsonValue, QBrush &value)
{
    if (!jsonValue.isObject()) {
        *errorString = "JSON value is not a brush object";
        return false;
    }
    QJsonObject jsonObject = jsonValue.toObject();
    QColor color;
    if (!jsonObject.contains("color") || !toColor(errorString, jsonObject.value("color"), color)) {
        return false;
    }
    value.setStyle(Qt::SolidPattern);
    value.setColor(color);
    return true;
}

QJsonArray fromPoint(const QPointF &value)
{
    QJsonArray jsonArray;
    jsonArray.append(QJsonValue(value.x()));
    jsonArray.append(QJsonValue(value.y()));
    return jsonArray;
}

QJsonArray fromPointList(const QList<QPointF> &value)
{
    QJsonArray jsonArray;
    for (int i = 0; i < value.count(); i++) {
        jsonArray.append(fromPoint(value.at(i)));
    }
    return jsonArray;
}

QJsonValue fromRect(const QRectF &value)
{
    return fromPointList(QList<QPointF>() << value.topLeft() << value.bottomRight());
}

QJsonValue fromLine(const QLineF &value)
{
    return fromPointList(QList<QPointF>() << value.p1() << value.p2());
}

QJsonValue fromSize(const QSizeF &value)
{
    return fromPoint(QPointF(value.width(), value.height()));
}

QJsonValue fromInt(int value)
{
    return QJsonValue(value);
}

QJsonValue fromBool(bool value)
{
    return QJsonValue(value);
}

QJsonValue fromReal(qreal value)
{
    return QJsonValue(value);
}


} // namespace Json
