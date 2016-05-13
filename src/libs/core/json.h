#ifndef JSON_H
#define JSON_H

#include "core/core_global.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QPointF>
#include <QLineF>
#include <QRectF>
#include <QPolygonF>
#include <QList>
#include <QSizeF>
#include <QPen>
#include <QBrush>


namespace Json {

CORE_EXPORT bool toPoint(QString *errorString, const QJsonValue &jsonValue, QPointF &value);
CORE_EXPORT bool toPointList(QString *errorString, const QJsonValue &jsonValue,
                             QList<QPointF> &value);
CORE_EXPORT bool toRect(QString *errorString, const QJsonValue &jsonValue, QRectF &value);
CORE_EXPORT bool toPolygon(QString *errorString, const QJsonValue &jsonValue, QPolygonF &value);
CORE_EXPORT bool toLine(QString *errorString, const QJsonValue &jsonValue, QLineF &value);
CORE_EXPORT bool toSize(QString *errorString, const QJsonValue &jsonValue, QSizeF &value);
CORE_EXPORT bool toInt(QString *errorString, const QJsonValue &jsonValue, int &value);
CORE_EXPORT bool toIntList(QString *errorString, const QJsonValue &jsonValue, QList<int> &value);
CORE_EXPORT bool toBool(QString *errorString, const QJsonValue &jsonValue, bool &value);
CORE_EXPORT bool toReal(QString *errorString, const QJsonValue &jsonValue, qreal &value);
CORE_EXPORT bool toColor(QString *errorString, const QJsonValue &jsonValue, QColor &value);
CORE_EXPORT bool toPen(QString *errorString, const QJsonValue &jsonValue, QPen &value);
CORE_EXPORT bool toBrush(QString *errorString, const QJsonValue &jsonValue, QBrush &value);
CORE_EXPORT bool toString(QString *errorString, const QJsonValue &jsonValue, QString &value);

CORE_EXPORT QJsonArray fromPoint(const QPointF &value);
CORE_EXPORT QJsonArray fromPointList(const QList<QPointF> &value);
CORE_EXPORT QJsonValue fromRect(const QRectF &value);
CORE_EXPORT QJsonValue fromPolygon(const QPolygonF &value);
CORE_EXPORT QJsonValue fromLine(const QLineF &value);
CORE_EXPORT QJsonValue fromSize(const QSizeF &value);
CORE_EXPORT QJsonValue fromInt(int value);
CORE_EXPORT QJsonValue fromBool(bool value);
CORE_EXPORT QJsonValue fromReal(qreal value);

}

#endif // CORE_JSON_H
