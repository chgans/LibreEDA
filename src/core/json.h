#ifndef JSON_H
#define JSON_H

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QPointF>
#include <QLineF>
#include <QRectF>
#include <QList>
#include <QSizeF>
#include <QPen>
#include <QBrush>


namespace Json
{

bool toPoint(QString *errorString, const QJsonValue &jsonValue, QPointF &value);
bool toPointList(QString *errorString, const QJsonValue &jsonValue, QList<QPointF> &value);
bool toRect(QString *errorString, const QJsonValue &jsonValue, QRectF &value);
bool toLine(QString *errorString, const QJsonValue &jsonValue, QLineF &value);
bool toSize(QString *errorString, const QJsonValue &jsonValue, QSizeF &value);
bool toInt(QString *errorString, const QJsonValue &jsonValue, int &value);
bool toIntList(QString *errorString, const QJsonValue &jsonValue, QList<int> &value);
bool toBool(QString *errorString, const QJsonValue &jsonValue, bool &value);
bool toReal(QString *errorString, const QJsonValue &jsonValue, qreal &value);
bool toColor(QString *errorString, const QJsonValue &jsonValue, QColor &value);
bool toPen(QString *errorString, const QJsonValue &jsonValue, QPen &value);
bool toBrush(QString *errorString, const QJsonValue &jsonValue, QBrush &value);

QJsonArray fromPoint(const QPointF &value);
QJsonArray fromPointList(const QList<QPointF> &value);
QJsonValue fromRect(const QRectF &value);
QJsonValue fromLine(const QLineF &value);
QJsonValue fromSize(const QSizeF &value);
QJsonValue fromInt(int value);
QJsonValue fromBool(bool value);
QJsonValue fromReal(qreal value);

}

#endif // JSON_H
